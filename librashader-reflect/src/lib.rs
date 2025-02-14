//! Shader reflection and cross-compilation for librashader.
//!
//! ## Usage
//!
//! librashader-reflect requires the `type_alias_impl_trait` nightly feature. You should choose your
//! target shading language, and a compilation type.
//!
//! ```rust
//! #![feature(type_alias_impl_trait)]
//!
//! use std::error::Error;
//! use librashader_preprocess::ShaderSource;
//! use librashader_presets::ShaderPreset;
//! use librashader_reflect::back::{CompileReflectShader, FromCompilation};
//! use librashader_reflect::back::targets::SPIRV;
//! use librashader_reflect::front::GlslangCompilation;
//! use librashader_reflect::reflect::presets::{CompilePresetTarget, ShaderPassArtifact};
//! use librashader_reflect::reflect::semantics::ShaderSemantics;
//! type Artifact = impl CompileReflectShader<SPIRV, GlslangCompilation>;
//! type ShaderPassMeta = ShaderPassArtifact<Artifact>;
//!
//! // Compile single shader
//! pub fn compile_spirv(
//!         source: &ShaderSource,
//!     ) -> Result<Artifact, Box<dyn Error>>
//! {
//!     let compilation = GlslangCompilation::compile(&source)?;
//!     let spirv = SPIRV::from_compilation(artifact)?;
//!     Ok(spirv)
//! }
//!
//! // Compile preset
//! pub fn compile_preset(preset: ShaderPreset) -> Result<(Vec<ShaderPassMeta>, ShaderSemantics), Box<dyn Error>>
//! {
//!     let (passes, semantics) = SPIRV::compile_preset_passes::<GlslangCompilation, Box<dyn Error>>(
//!     preset.shaders, &preset.textures)?;
//!     Ok((passes, semantics))
//! }
//! ```
//!
//! ## What's with all the traits?
//! librashader-reflect is designed to be compiler-agnostic. In the future, we will allow usage of
//! [naga](https://docs.rs/naga/latest/naga/index.html), a pure-Rust shader compiler, when it has
//! matured enough to support [the features librashader needs](https://github.com/gfx-rs/naga/issues/1012).
//!
//! In the meanwhile, the only supported compilation type is [GlslangCompilation](crate::front::GlslangCompilation),
//! which does transpilation via [shaderc](https://github.com/google/shaderc) and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross).
#![feature(type_alias_impl_trait)]
#![feature(impl_trait_in_assoc_type)]
#![feature(let_chains)]

/// Shader codegen backends.
pub mod back;
/// Error types.
pub mod error;
/// Shader frontend parsers.
pub mod front;
/// Shader reflection.
pub mod reflect;
