use bitflags::bitflags;
use rustc_hash::FxHashMap;
use std::str::FromStr;

/// The maximum number of bindings allowed in a shader.
pub const MAX_BINDINGS_COUNT: u32 = 16;
/// The maximum size of the push constant range.
pub const MAX_PUSH_BUFFER_SIZE: u32 = 128;

/// The type of a uniform.
#[derive(Debug, Ord, PartialOrd, Eq, PartialEq, Copy, Clone, Hash)]
pub enum UniformType {
    /// A matrix of 4x4 floats (`mat4`).
    Mat4,
    /// A vector of 4 floats (`vec4`).
    Vec4,
    /// An unsigned integer (`uint`).
    Unsigned,
    /// A signed integer (`int`).
    Signed,
    /// A floating point number (`float`).
    Float,
}

/// Unique semantics are builtin uniforms passed by the shader runtime
/// that are always available.
#[derive(Debug, Ord, PartialOrd, Eq, PartialEq, Copy, Clone, Hash)]
#[repr(i32)]
pub enum UniqueSemantics {
    // mat4, MVP
    /// The Model View Projection matrix for the frame.
    MVP = 0,
    // vec4, viewport size of current pass
    /// The viewport size of the current pass.
    Output = 1,
    // vec4, viewport size of final pass
    /// The viewport size of the final pass.
    FinalViewport = 2,
    // uint, frame count with modulo
    /// The frame count, possibly with shader-defined modulo.
    FrameCount = 3,
    // int, frame direction
    /// The frame direction.
    FrameDirection = 4,
    /// A user defined float parameter.
    // float, user defined parameter, array
    FloatParameter = 5,
}

impl UniqueSemantics {
    /// Produce a `Semantic` for this `UniqueSemantics`.
    pub const fn semantics(self) -> Semantic<UniqueSemantics, ()> {
        Semantic {
            semantics: self,
            index: (),
        }
    }

    /// Get the type of the uniform when bound.
    pub const fn binding_type(&self) -> UniformType {
        match self {
            UniqueSemantics::MVP => UniformType::Mat4,
            UniqueSemantics::Output => UniformType::Vec4,
            UniqueSemantics::FinalViewport => UniformType::Vec4,
            UniqueSemantics::FrameCount => UniformType::Unsigned,
            UniqueSemantics::FrameDirection => UniformType::Signed,
            UniqueSemantics::FloatParameter => UniformType::Float,
        }
    }
}

/// Texture semantics relate to input or output textures.
///
/// Texture semantics are used to relate both texture samplers and `*Size` uniforms.
#[derive(Debug, Ord, PartialOrd, Eq, PartialEq, Copy, Clone, Hash)]
#[repr(i32)]
pub enum TextureSemantics {
    /// The original input of the filter chain.
    Original = 0,
    /// The input from the previous shader pass, or the input on the first shader pass.
    Source = 1,
    /// The input frames from previous frames.
    OriginalHistory = 2,
    /// The output from previous shader passes in the same frame.
    PassOutput = 3,
    /// The output from previous shader passes in the previous frame.
    PassFeedback = 4,
    /// A user provided lookup texture.
    User = 5,
}

impl TextureSemantics {
    pub(crate) const TEXTURE_SEMANTICS: [TextureSemantics; 6] = [
        TextureSemantics::Source,
        // originalhistory needs to come first, otherwise
        // the name lookup implementation will prioritize Original
        // when reflecting semantics.
        TextureSemantics::OriginalHistory,
        TextureSemantics::Original,
        TextureSemantics::PassOutput,
        TextureSemantics::PassFeedback,
        TextureSemantics::User,
    ];

    /// Get the name of the size uniform for this semantics when bound.
    pub fn size_uniform_name(&self) -> &'static str {
        match self {
            TextureSemantics::Original => "OriginalSize",
            TextureSemantics::Source => "SourceSize",
            TextureSemantics::OriginalHistory => "OriginalHistorySize",
            TextureSemantics::PassOutput => "PassOutputSize",
            TextureSemantics::PassFeedback => "PassFeedbackSize",
            TextureSemantics::User => "UserSize",
        }
    }

    /// Get the name of the texture sampler for this semantics when bound.
    pub fn texture_name(&self) -> &'static str {
        match self {
            TextureSemantics::Original => "Original",
            TextureSemantics::Source => "Source",
            TextureSemantics::OriginalHistory => "OriginalHistory",
            TextureSemantics::PassOutput => "PassOutput",
            TextureSemantics::PassFeedback => "PassFeedback",
            TextureSemantics::User => "User",
        }
    }

    /// Returns whether or not textures of this semantics are indexed or unique.
    ///
    /// Only Original and Source are unique, all other textures can be indexed.
    pub fn is_indexed(&self) -> bool {
        !matches!(self, TextureSemantics::Original | TextureSemantics::Source)
    }

    /// Produce a `Semantic` for this `TextureSemantics` of the given index.
    pub const fn semantics(self, index: usize) -> Semantic<TextureSemantics> {
        Semantic {
            semantics: self,
            index,
        }
    }
}

pub(crate) struct TypeInfo {
    pub size: u32,
    pub columns: u32,
}

pub(crate) trait ValidateTypeSemantics<T> {
    fn validate_type(&self, ty: &T) -> Option<TypeInfo>;
}

/// A unit of unique or indexed semantic.
#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
pub struct Semantic<T, I = usize> {
    /// The semantics of this unit.
    pub semantics: T,
    /// The index of the semantic if not unique.
    pub index: I,
}

bitflags! {
    /// The pipeline stage for which a uniform is bound.
    pub struct BindingStage: u8 {
        const NONE = 0b00000000;
        const VERTEX = 0b00000001;
        const FRAGMENT = 0b00000010;
    }
}

/// Reflection information for the Uniform Buffer
#[derive(Debug)]
pub struct UboReflection {
    /// The binding point for this UBO.
    pub binding: u32,
    /// The size of the UBO buffer. UBO sizes returned by reflection is always aligned to a 16 byte boundary.
    pub size: u32,
    /// The mask indicating for which stages the UBO should be bound.
    pub stage_mask: BindingStage,
}

/// Reflection information for the Push Constant Block
#[derive(Debug)]
pub struct PushReflection {
    /// The size of the Push Constant range. The size returned by reflection is always aligned to a 16 byte boundary.
    pub size: u32,
    /// The mask indicating for which stages the Push Constant range should be bound.
    pub stage_mask: BindingStage,
}

/// The offset of a uniform member.
///
/// A uniform can be bound to both the UBO, or as a Push Constant.
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub struct MemberOffset {
    /// The offset of the uniform member within the UBO.
    pub ubo: Option<usize>,
    /// The offset of the uniform member within the Push Constant range.
    pub push: Option<usize>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// The block where a uniform member is located.
pub enum UniformMemberBlock {
    /// The offset is for a UBO.
    Ubo,
    /// The offset is for a push constant block.
    PushConstant,
}

impl UniformMemberBlock {
    /// A list of valid member block types.
    pub const TYPES: [UniformMemberBlock; 2] =
        [UniformMemberBlock::Ubo, UniformMemberBlock::PushConstant];
}

impl MemberOffset {
    pub(crate) fn new(off: usize, ty: UniformMemberBlock) -> Self {
        match ty {
            UniformMemberBlock::Ubo => MemberOffset {
                ubo: Some(off),
                push: None,
            },
            UniformMemberBlock::PushConstant => MemberOffset {
                ubo: None,
                push: Some(off),
            },
        }
    }

    pub fn offset(&self, ty: UniformMemberBlock) -> Option<usize> {
        match ty {
            UniformMemberBlock::Ubo => self.ubo,
            UniformMemberBlock::PushConstant => self.push,
        }
    }

    pub(crate) fn offset_mut(&mut self, ty: UniformMemberBlock) -> &mut Option<usize> {
        match ty {
            UniformMemberBlock::Ubo => &mut self.ubo,
            UniformMemberBlock::PushConstant => &mut self.push,
        }
    }
}

/// Reflection information about a non-texture related uniform variable.
#[derive(Debug)]
pub struct VariableMeta {
    // this might bite us in the back because retroarch keeps separate UBO/push offsets.. eh
    /// The offset of this variable uniform.
    pub offset: MemberOffset,
    /// The size of the uniform.
    pub size: u32,
    /// The name of the uniform.
    pub id: String,
}

/// Reflection information about a texture size uniform variable.
#[derive(Debug)]
pub struct TextureSizeMeta {
    // this might bite us in the back because retroarch keeps separate UBO/push offsets..
    /// The offset of this size uniform.
    pub offset: MemberOffset,
    /// The mask indicating for which stages the texture size uniform should be bound.
    pub stage_mask: BindingStage,
    /// The name of the uniform.
    pub id: String,
}

/// Reflection information about texture samplers.
#[derive(Debug)]
pub struct TextureBinding {
    /// The binding index of the texture.
    pub binding: u32,
}

/// Reflection information about a shader.
#[derive(Debug)]
pub struct ShaderReflection {
    /// Reflection information about the UBO for this shader.
    pub ubo: Option<UboReflection>,
    /// Reflection information about the Push Constant range for this shader.
    pub push_constant: Option<PushReflection>,
    /// Metadata about the bindings required for this shader.
    pub meta: BindingMeta,
}

/// Metadata about a uniform variable.
pub trait UniformMeta {
    /// The offset of this uniform.
    fn offset(&self) -> MemberOffset;
    /// The name of this uniform in the shader.
    fn id(&self) -> &str;
}

impl UniformMeta for VariableMeta {
    fn offset(&self) -> MemberOffset {
        self.offset
    }

    fn id(&self) -> &str {
        &self.id
    }
}

impl UniformMeta for TextureSizeMeta {
    fn offset(&self) -> MemberOffset {
        self.offset
    }
    fn id(&self) -> &str {
        &self.id
    }
}

/// A trait for maps that can return texture semantic units.
pub trait TextureSemanticMap {
    /// Get the texture semantic for the given variable name.
    fn get_texture_semantic(&self, name: &str) -> Option<Semantic<TextureSemantics>>;
}

impl TextureSemanticMap for FxHashMap<String, UniformSemantic> {
    fn get_texture_semantic(&self, name: &str) -> Option<Semantic<TextureSemantics>> {
        match self.get(name) {
            None => {
                if let Some(semantics) = TextureSemantics::TEXTURE_SEMANTICS
                    .iter()
                    .find(|f| name.starts_with(f.size_uniform_name()))
                {
                    if semantics.is_indexed() {
                        let index = &name[semantics.size_uniform_name().len()..];
                        let Ok(index) = usize::from_str(index) else {
                            return None;
                        };
                        return Some(Semantic {
                            semantics: *semantics,
                            index,
                        });
                    } else if name == semantics.size_uniform_name() {
                        return Some(Semantic {
                            semantics: *semantics,
                            index: 0,
                        });
                    }
                }
                None
            }
            Some(UniformSemantic::Unique(_)) => None,
            Some(UniformSemantic::Texture(texture)) => Some(*texture),
        }
    }
}

impl TextureSemanticMap for FxHashMap<String, Semantic<TextureSemantics>> {
    fn get_texture_semantic(&self, name: &str) -> Option<Semantic<TextureSemantics>> {
        match self.get(name) {
            None => {
                if let Some(semantics) = TextureSemantics::TEXTURE_SEMANTICS
                    .iter()
                    .find(|f| name.starts_with(f.texture_name()))
                {
                    if semantics.is_indexed() {
                        let index = &name[semantics.texture_name().len()..];
                        let Ok(index) = usize::from_str(index) else {
                            return None;
                        };
                        return Some(Semantic {
                            semantics: *semantics,
                            index,
                        });
                    } else if name == semantics.texture_name() {
                        return Some(Semantic {
                            semantics: *semantics,
                            index: 0,
                        });
                    }
                }
                None
            }
            Some(texture) => Some(*texture),
        }
    }
}

/// A trait for maps that can return unique semantic units.
pub trait UniqueSemanticMap {
    /// Get the unique semantic for the given variable name.
    fn get_unique_semantic(&self, name: &str) -> Option<Semantic<UniqueSemantics, ()>>;
}

impl UniqueSemanticMap for FxHashMap<String, UniformSemantic> {
    fn get_unique_semantic(&self, name: &str) -> Option<Semantic<UniqueSemantics, ()>> {
        match self.get(name) {
            // existing uniforms in the semantic map have priority
            None => match name {
                "MVP" => Some(Semantic {
                    semantics: UniqueSemantics::MVP,
                    index: (),
                }),
                "OutputSize" => Some(Semantic {
                    semantics: UniqueSemantics::Output,
                    index: (),
                }),
                "FinalViewportSize" => Some(Semantic {
                    semantics: UniqueSemantics::FinalViewport,
                    index: (),
                }),
                "FrameCount" => Some(Semantic {
                    semantics: UniqueSemantics::FrameCount,
                    index: (),
                }),
                "FrameDirection" => Some(Semantic {
                    semantics: UniqueSemantics::FrameDirection,
                    index: (),
                }),
                _ => None,
            },
            Some(UniformSemantic::Unique(variable)) => Some(*variable),
            Some(UniformSemantic::Texture(_)) => None,
        }
    }
}

/// Semantic assignment of a shader uniform to filter chain semantics.
#[derive(Debug, Clone)]
pub enum UniformSemantic {
    /// A unique semantic.
    Unique(Semantic<UniqueSemantics, ()>),
    /// A texture related semantic.
    Texture(Semantic<TextureSemantics>),
}

/// The runtime provided maps of uniform and texture variables to filter chain semantics.
#[derive(Debug, Clone)]
pub struct ShaderSemantics {
    /// A map of uniform names to filter chain semantics.
    pub uniform_semantics: FxHashMap<String, UniformSemantic>,
    /// A map of texture names to filter chain semantics.
    pub texture_semantics: FxHashMap<String, Semantic<TextureSemantics>>,
}

/// The binding of a uniform after the shader has been linked.
///
/// Used in combination with [`MemberOffset`](crate::reflect::semantics::MemberOffset) to keep track
/// of semantics at each frame pass.
#[derive(Debug, Clone, Eq, Hash, PartialEq)]
pub enum UniformBinding {
    /// A user parameter (`float`) binding.
    Parameter(String),
    /// A known semantic binding.
    SemanticVariable(UniqueSemantics),
    /// A texture size (`float4`) binding.
    TextureSize(Semantic<TextureSemantics>),
}

impl From<UniqueSemantics> for UniformBinding {
    fn from(value: UniqueSemantics) -> Self {
        UniformBinding::SemanticVariable(value)
    }
}

impl From<Semantic<TextureSemantics>> for UniformBinding {
    fn from(value: Semantic<TextureSemantics>) -> Self {
        UniformBinding::TextureSize(value)
    }
}

/// Reflection metadata about the various bindings for this shader.
#[derive(Debug, Default)]
pub struct BindingMeta {
    /// A map of parameter names to uniform binding metadata.
    pub parameter_meta: FxHashMap<String, VariableMeta>,
    /// A map of unique semantics to uniform binding metadata.
    pub unique_meta: FxHashMap<UniqueSemantics, VariableMeta>,
    /// A map of texture semantics to texture binding points.
    pub texture_meta: FxHashMap<Semantic<TextureSemantics>, TextureBinding>,
    /// A map of texture semantics to texture size uniform binding metadata.
    pub texture_size_meta: FxHashMap<Semantic<TextureSemantics>, TextureSizeMeta>,
}
