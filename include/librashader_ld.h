/*
librashader_ld.h
SPDX-License-Identifier: MIT
This file is part of the librashader C headers.

Copyright 2022 chyyran

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __LIBRASHADER_LD_H__
#define __LIBRASHADER_LD_H__
#pragma once

// Uncomment the following defines to activate runtimes.
 
// #define LIBRA_RUNTIME_OPENGL
// #define LIBRA_RUNTIME_VULKAN

// #if defined(_WIN32)
// #define LIBRA_RUNTIME_D3D11
// #define LIBRA_RUNTIME_D3D12
// #endif

#if defined(_WIN32)
#include <windows.h>
#define _LIBRASHADER_ASSIGN(HMOD, INSTANCE, NAME)       \
    {                                                           \
        FARPROC address = GetProcAddress(HMOD, "libra_" #NAME); \
        if (address != NULL) {                                  \
            (INSTANCE).NAME = (PFN_libra_##NAME)address;        \
        }                                                       \
    }
typedef HMODULE _LIBRASHADER_IMPL_HANDLE;
#define _LIBRASHADER_LOAD LoadLibraryW(L"librashader.dll")

#elif defined(__linux__)
#include <dlfcn.h>
#define _LIBRASHADER_ASSIGN(HMOD, INSTANCE, NAME)        \
    {                                                    \
        void *address = dlsym(HMOD, "libra_" #NAME);     \
        if (address != NULL) {                           \
            (INSTANCE).NAME = (PFN_libra_##NAME)address; \
        }                                                \
    }
typedef void* _LIBRASHADER_IMPL_HANDLE;
#define _LIBRASHADER_LOAD dlopen("librashader.so", RTLD_LAZY)
#endif

#include "librashader.h"

size_t __librashader__noop_instance_abi_version() { return 0; }

size_t __librashader__noop_instance_api_version() { return 0; }

LIBRA_ERRNO __librashader__noop_error_errno(libra_error_t error) {
    return LIBRA_ERRNO_UNKNOWN_ERROR;
}

int32_t __librashader__noop_error_print(libra_error_t error) { return 1; }

int32_t __librashader__noop_error_free(libra_error_t *error) { return 1; }

int32_t __librashader__noop_error_write(libra_error_t error, char **out) {
    return 1;
}

int32_t __librashader__noop_error_free_string(char **out) { return 1; }

libra_error_t __librashader__noop_preset_create(const char *filename,
                                                libra_shader_preset_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_preset_free(libra_shader_preset_t *preset) {
    return NULL;
}

libra_error_t __librashader__noop_preset_set_param(
    libra_shader_preset_t *preset, const char *name, float value) {
    return NULL;
}

libra_error_t __librashader__noop_preset_get_param(
    libra_shader_preset_t *preset, const char *name, float *value) {
    return NULL;
}

libra_error_t __librashader__noop_preset_print(libra_shader_preset_t *preset) {
    return NULL;
}
libra_error_t __librashader__noop_preset_get_runtime_params(
    libra_shader_preset_t *preset, struct libra_preset_param_list_t *out) {
    return NULL;
}
libra_error_t __librashader__noop_preset_free_runtime_params(struct libra_preset_param_list_t out) {
    return NULL;
}
#if defined(LIBRA_RUNTIME_OPENGL)
libra_error_t __librashader__noop_gl_init_context(libra_gl_loader_t loader) {
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_create(
    libra_shader_preset_t *preset, const struct filter_chain_gl_opt_t *options,
    libra_gl_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_frame(
    libra_gl_filter_chain_t *chain, size_t frame_count,
    struct libra_source_image_gl_t image, struct libra_viewport_t viewport,
    struct libra_output_framebuffer_gl_t out, const float *mvp,
    const struct frame_gl_opt_t *opt) {
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_free(
    libra_gl_filter_chain_t *chain) {
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_set_param(
    libra_gl_filter_chain_t *chain, const char *param_name, float value) {
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_get_param(
    libra_gl_filter_chain_t *chain, const char *param_name, float *out) {
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_set_active_pass_count(
    libra_gl_filter_chain_t *chain, uint32_t value) {
    return NULL;
}

libra_error_t __librashader__noop_gl_filter_chain_get_active_pass_count(
    libra_gl_filter_chain_t *chain, uint32_t *out) {
    return NULL;
}
#endif

#if defined(LIBRA_RUNTIME_VULKAN)
libra_error_t __librashader__noop_vk_filter_chain_create(
    libra_shader_preset_t *preset, struct libra_device_vk_t vulkan, 
    const struct filter_chain_vk_opt_t *options, libra_vk_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_create_deferred(
    libra_shader_preset_t *preset, struct libra_device_vk_t vulkan,
    VkCommandBuffer command_buffer, const struct filter_chain_vk_opt_t *options, 
    libra_vk_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_frame(
    libra_vk_filter_chain_t *chain, VkCommandBuffer command_buffer,
    size_t frame_count, struct libra_source_image_vk_t image,
    struct libra_viewport_t viewport, struct libra_output_image_vk_t out,
    const float *mvp, const struct frame_vk_opt_t *opt) {
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_free(
    libra_vk_filter_chain_t *chain) {
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_set_param(
    libra_vk_filter_chain_t *chain, const char *param_name, float value) {
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_get_param(
    libra_vk_filter_chain_t *chain, const char *param_name, float *out) {
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_set_active_pass_count(
    libra_vk_filter_chain_t *chain, uint32_t value) {
    return NULL;
}

libra_error_t __librashader__noop_vk_filter_chain_get_active_pass_count(
    libra_vk_filter_chain_t *chain, uint32_t *out) {
    return NULL;
}
#endif

#if defined(LIBRA_RUNTIME_D3D11)
libra_error_t __librashader__noop_d3d11_filter_chain_create(
    libra_shader_preset_t *preset, ID3D11Device *device,
    const struct filter_chain_d3d11_opt_t *options, 
    libra_d3d11_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_create_deferred(
    libra_shader_preset_t *preset, ID3D11Device *device, ID3D11DeviceContext *device_context,
    const struct filter_chain_d3d11_opt_t *options,
    libra_d3d11_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_frame(
    libra_d3d11_filter_chain_t *chain,
    ID3D11DeviceContext *device_context, size_t frame_count,
    struct libra_source_image_d3d11_t image, struct libra_viewport_t viewport,
    ID3D11RenderTargetView *out, const float *mvp,
    const struct frame_d3d11_opt_t *opt) {
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_free(
    libra_d3d11_filter_chain_t *chain) {
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_set_param(
    libra_d3d11_filter_chain_t *chain, const char *param_name, float value) {
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_get_param(
    libra_d3d11_filter_chain_t *chain, const char *param_name, float *out) {
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_set_active_pass_count(
    libra_d3d11_filter_chain_t *chain, uint32_t value) {
    return NULL;
}

libra_error_t __librashader__noop_d3d11_filter_chain_get_active_pass_count(
    libra_d3d11_filter_chain_t *chain, uint32_t *out) {
    return NULL;
}
#endif


#if defined(LIBRA_RUNTIME_D3D12)
libra_error_t __librashader__noop_d3d12_filter_chain_create(
    libra_shader_preset_t *preset, ID3D12Device *device,
    const struct filter_chain_d3d12_opt_t *options,
    libra_d3d12_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_create_deferred(
    libra_shader_preset_t *preset, ID3D12Device *device,
    ID3D12GraphicsCommandList *command_list,
    const struct filter_chain_d3d12_opt_t *options,
    libra_d3d12_filter_chain_t *out) {
    *out = NULL;
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_frame(
    libra_d3d12_filter_chain_t *chain,
    ID3D12GraphicsCommandList *command_list, size_t frame_count,
    struct libra_source_image_d3d12_t image, struct libra_viewport_t viewport,
    struct libra_output_image_d3d12_t out, const float *mvp,
    const struct frame_d3d12_opt_t *opt) {
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_free(
    libra_d3d12_filter_chain_t *chain) {
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_set_param(
    libra_d3d12_filter_chain_t *chain, const char *param_name, float value) {
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_get_param(
    libra_d3d12_filter_chain_t *chain, const char *param_name, float *out) {
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_set_active_pass_count(
    libra_d3d12_filter_chain_t *chain, uint32_t value) {
    return NULL;
}

libra_error_t __librashader__noop_d3d12_filter_chain_get_active_pass_count(
    libra_d3d12_filter_chain_t *chain, uint32_t *out) {
    return NULL;
}
#endif
typedef struct libra_instance_t {
    /// Get the supported ABI version of the loaded instance.
    ///
    /// The null instance has ABI version 0. Any valid loaded
    /// instance must have ABI version greater than or equal to 1.
    PFN_libra_instance_abi_version instance_abi_version;
   
    /// Get the supported API version of the loaded instance.
    ///
    /// The null instance has API version 0.
    PFN_libra_instance_api_version instance_api_version;

    /// Load a preset.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety
    ///  - `filename` must be either null or a valid, aligned pointer to a
    ///  string path to the shader preset.
    ///  - `out` must be either null, or an aligned pointer to an uninitialized
    ///  or invalid `libra_shader_preset_t`.
    /// ## Returns
    ///  - If any parameters are null, `out` is unchanged, and this function
    ///  returns `LIBRA_ERR_INVALID_PARAMETER`.
    PFN_libra_preset_create preset_create;

    /// Free the preset.
    ///
    /// If `preset` is null, this function does nothing. The resulting value in
    /// `preset` then becomes null.
    ///
    /// ## Safety
    /// - `preset` must be a valid and aligned pointer to a shader preset.
    PFN_libra_preset_free preset_free;

    /// Set the value of the parameter in the preset.
    ///
    /// ## Safety
    /// - `preset` must be null or a valid and aligned pointer to a shader
    /// preset.
    /// - `name` must be null or a valid and aligned pointer to a string.
    PFN_libra_preset_set_param preset_set_param;

    /// Get the value of the parameter as set in the preset.
    ///
    /// ## Safety
    /// - `preset` must be null or a valid and aligned pointer to a shader
    /// preset.
    /// - `name` must be null or a valid and aligned pointer to a string.
    /// - `value` may be a pointer to a uninitialized `float`.
    PFN_libra_preset_get_param preset_get_param;

    /// Pretty print the shader preset.
    ///
    /// ## Safety
    /// - `preset` must be null or a valid and aligned pointer to a shader
    /// preset.
    PFN_libra_preset_print preset_print;

    /// Get a list of runtime parameter names.
    ///
    /// ## Safety
    /// - `preset` must be null or a valid and aligned pointer to a shader
    /// preset.
    /// - `out` must be an aligned pointer to a `libra_preset_parameter_list_t`.
    /// - The output struct should be treated as immutable. Mutating any struct
    /// fields
    ///   in the returned struct may at best cause memory leaks, and at worse
    ///   cause undefined behaviour when later freed.
    /// - It is safe to call `libra_preset_get_runtime_params` multiple times,
    /// however
    ///   the output struct must only be freed once per call.
    PFN_libra_preset_get_runtime_params preset_get_runtime_params;

    /// Free the runtime parameters.
    ///
    /// Unlike the other `free` functions provided by librashader,
    /// `libra_preset_free_runtime_params` takes the struct directly.
    /// The caller must take care to maintain the lifetime of any pointers
    /// contained within the input `libra_preset_param_list_t`.
    ///
    /// ## Safety
    /// - Any pointers rooted at `parameters` becomes invalid after this
    /// function returns,
    ///   including any strings accessible via the input
    ///   `libra_preset_param_list_t`. The caller must ensure that there are no
    ///   live pointers, aliased or unaliased, to data accessible via the input
    ///   `libra_preset_param_list_t`.
    ///
    /// - Accessing any data pointed to via the input
    /// `libra_preset_param_list_t` after it
    ///   has been freed is a use-after-free and is immediate undefined
    ///   behaviour.
    ///
    /// - If any struct fields of the input `libra_preset_param_list_t` was
    /// modified from
    ///   their values given after `libra_preset_get_runtime_params`, this may
    ///   result in undefined behaviour.
    PFN_libra_preset_free_runtime_params preset_free_runtime_params;

    
    /// Get the error code corresponding to this error object.
    ///
    /// ## Safety
    ///   - `error` must be valid and initialized.
    PFN_libra_error_errno error_errno;

    /// Print the error message.
    ///
    /// If `error` is null, this function does nothing and returns 1. Otherwise,
    /// this function returns 0.
    /// ## Safety
    ///   - `error` must be a valid and initialized instance of `libra_error_t`.
    PFN_libra_error_print error_print;

    /// Frees any internal state kept by the error.
    ///
    /// If `error` is null, this function does nothing and returns 1. Otherwise,
    /// this function returns 0. The resulting error object becomes null.
    /// ## Safety
    ///   - `error` must be null or a pointer to a valid and initialized
    ///   instance of `libra_error_t`.
    PFN_libra_error_free error_free;

    /// Writes the error message into `out`
    ///
    /// If `error` is null, this function does nothing and returns 1. Otherwise,
    /// this function returns 0.
    /// ## Safety
    ///   - `error` must be a valid and initialized instance of `libra_error_t`.
    ///   - `out` must be a non-null pointer. The resulting string must not be
    ///   modified.
    PFN_libra_error_write error_write;

    /// Frees an error string previously allocated by `libra_error_write`.
    ///
    /// After freeing, the pointer will be set to null.
    /// ## Safety
    ///   - If `libra_error_write` is not null, it must point to a string
    ///   previously returned by `libra_error_write`.
    ///     Attempting to free anything else, including strings or objects from
    ///     other librashader functions, is immediate Undefined Behaviour.
    PFN_libra_error_free_string error_free_string;

#if defined(LIBRA_RUNTIME_OPENGL)
    /// Initialize the OpenGL Context for librashader.
    ///
    /// ## Safety
    /// Attempting to create a filter chain will fail if the context is not initialized.
    ///
    /// Reinitializing the OpenGL context with a different loader immediately
    /// invalidates previous filter chain objects, and drawing with them causes
    /// immediate undefined behaviour.
    PFN_libra_gl_init_context gl_init_context;

    /// Create the filter chain given the shader preset.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety:
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    PFN_libra_gl_filter_chain_create gl_filter_chain_create;

    /// Draw a frame with the given parameters for the given filter chain.
    ///
    /// ## Safety
    /// - `chain` may be null, invalid, but not uninitialized. If `chain` is
    /// null or invalid, this
    ///    function will return an error.
    /// - `mvp` may be null, or if it is not null, must be an aligned pointer to
    /// 16 consecutive `float`
    ///    values for the model view projection matrix.
    /// - `opt` may be null, or if it is not null, must be an aligned pointer to
    /// a valid `frame_gl_opt_t`
    ///    struct.
    PFN_libra_gl_filter_chain_frame gl_filter_chain_frame;

    /// Free a GL filter chain.
    ///
    /// The resulting value in `chain` then becomes null.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_gl_filter_chain_t`.
    PFN_libra_gl_filter_chain_free gl_filter_chain_free;

    /// Gets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_gl_filter_chain_t`.
    PFN_libra_gl_filter_chain_get_active_pass_count
        gl_filter_chain_get_active_pass_count;

    /// Sets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_gl_filter_chain_t`.
    PFN_libra_gl_filter_chain_set_active_pass_count
        gl_filter_chain_set_active_pass_count;

    /// Gets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_gl_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_gl_filter_chain_get_param gl_filter_chain_get_param;

    /// Sets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_gl_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_gl_filter_chain_set_param gl_filter_chain_set_param;
#endif

#if defined(LIBRA_RUNTIME_VULKAN)
    /// Create the filter chain given the shader preset.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety:
    /// - The handles provided in `vulkan` must be valid for the command buffers
    /// that
    ///   `libra_vk_filter_chain_frame` will write to. Namely, the VkDevice must
    ///   have been
    ///    created with the `VK_KHR_dynamic_rendering` extension.
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    PFN_libra_vk_filter_chain_create vk_filter_chain_create;

    /// Create the filter chain given the shader preset deferring and GPU-side
    /// initialization to the caller. This function therefore requires no
    /// external synchronization of the device queue.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety:
    /// - The handles provided in `vulkan` must be valid for the command buffers
    /// that
    ///   `libra_vk_filter_chain_frame` will write to. Namely, the VkDevice must
    ///   have been
    ///    created with the `VK_KHR_dynamic_rendering` extension.
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    ///
    /// The provided command buffer must be ready for recording and contain no
    /// prior commands. The caller is responsible for ending the command buffer
    /// and immediately submitting it to a graphics queue. The command buffer
    /// must be completely executed before calling
    /// `libra_vk_filter_chain_frame`.
    PFN_libra_vk_filter_chain_create_deferred vk_filter_chain_create_deferred;

    /// Records rendering commands for a frame with the given parameters for the
    /// given filter chain
    /// to the input command buffer.
    ///
    /// librashader will not do any queue submissions.
    ///
    /// ## Safety
    /// - `libra_vk_filter_chain_frame` **must not be called within a
    /// RenderPass**.
    /// - `command_buffer` must be a valid handle to a `VkCommandBuffer` that is
    /// ready for recording.
    /// - `chain` may be null, invalid, but not uninitialized. If `chain` is
    /// null or invalid, this
    ///    function will return an error.
    /// - `mvp` may be null, or if it is not null, must be an aligned pointer to
    /// 16 consecutive `float`
    ///    values for the model view projection matrix.
    /// - `opt` may be null, or if it is not null, must be an aligned pointer to
    /// a valid `frame_vk_opt_t`
    ///    struct.
    PFN_libra_vk_filter_chain_frame vk_filter_chain_frame;

    /// Free a Vulkan filter chain.
    ///
    /// The resulting value in `chain` then becomes null.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_vk_filter_chain_t`.
    PFN_libra_vk_filter_chain_free vk_filter_chain_free;

    /// Gets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_vk_filter_chain_t`.
    PFN_libra_vk_filter_chain_get_active_pass_count
        vk_filter_chain_get_active_pass_count;

    /// Sets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_vk_filter_chain_t`.
    PFN_libra_vk_filter_chain_set_active_pass_count
        vk_filter_chain_set_active_pass_count;

    /// Gets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_vk_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_vk_filter_chain_get_param vk_filter_chain_get_param;

    /// Sets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_vk_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_vk_filter_chain_set_param vk_filter_chain_set_param;
#endif

#if defined(LIBRA_RUNTIME_D3D11)
    /// Create the filter chain given the shader preset.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety:
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    PFN_libra_d3d11_filter_chain_create d3d11_filter_chain_create;

    /// Create the filter chain given the shader preset, deferring and GPU-side
    /// initialization
    /// to the caller. This function is therefore requires no external
    /// synchronization of the immediate context, as long as the immediate
    /// context is not used as the input context, nor of the device, as long as
    /// the device is not single-threaded only.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety:
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `device` must not be null.
    /// - `device_context` not be null.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    ///
    /// The provided context must either be immediate, or immediately submitted
    /// after this function returns, **before drawing frames**, or lookup
    /// textures will fail to load and the filter chain will be in an invalid
    /// state.
    ///
    /// If the context is deferred, it must be ready for command recording, and
    /// have no prior commands recorded. No commands shall be recorded after,
    /// the caller must immediately call
    /// [`FinishCommandList`](https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-finishcommandlist)
    /// and execute the command list on the immediate context after this
    /// function returns.
    ///
    /// If the context is immediate, then access to the immediate context
    /// requires external synchronization.
    PFN_libra_d3d11_filter_chain_create_deferred d3d11_filter_chain_create_deferred;

    /// Draw a frame with the given parameters for the given filter chain.
    ///
    /// ## Safety
    /// - `chain` may be null, invalid, but not uninitialized. If `chain` is
    /// null or invalid, this
    ///    function will return an error.
    /// - `mvp` may be null, or if it is not null, must be an aligned pointer to
    /// 16 consecutive `float`
    ///    values for the model view projection matrix.
    /// - `opt` may be null, or if it is not null, must be an aligned pointer to
    /// a valid `frame_d3d11_opt_t`
    ///    struct.
    PFN_libra_d3d11_filter_chain_frame d3d11_filter_chain_frame;

    /// Free a D3D11 filter chain.
    ///
    /// The resulting value in `chain` then becomes null.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d11_filter_chain_t`.
    PFN_libra_d3d11_filter_chain_free d3d11_filter_chain_free;

    /// Gets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d11_filter_chain_t`.
    PFN_libra_d3d11_filter_chain_get_active_pass_count
        d3d11_filter_chain_get_active_pass_count;

    /// Sets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d11_filter_chain_t`.
    PFN_libra_d3d11_filter_chain_set_active_pass_count
        d3d11_filter_chain_set_active_pass_count;

    /// Gets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d11_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_d3d11_filter_chain_get_param d3d11_filter_chain_get_param;

    /// Sets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d11_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_d3d11_filter_chain_set_param d3d11_filter_chain_set_param;
#endif


#if defined(LIBRA_RUNTIME_D3D12)
    /// Create the filter chain given the shader preset.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to
    /// null. If this function returns an error, the state of `out` is
    /// unspecified.
    /// 
    /// ## Safety:
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    PFN_libra_d3d12_filter_chain_create d3d12_filter_chain_create;

    /// Create the filter chain given the shader preset deferring and GPU-side
    /// initialization
    /// to the caller. This function therefore requires no external
    /// synchronization of the device queue.
    ///
    /// The shader preset is immediately invalidated and must be recreated after
    /// the filter chain is created.
    ///
    /// If this function is not loaded, `out` will unconditionally be set to null. 
    /// If this function returns an error, the state of `out` is unspecified.
    /// 
    /// ## Safety:
    /// - `preset` must be either null, or valid and aligned.
    /// - `options` must be either null, or valid and aligned.
    /// - `device` must not be null.
    /// - `out` must be aligned, but may be null, invalid, or uninitialized.
    /// - `cmd` must not be null.
    ///
    /// The provided command list must be ready for recording and contain no
    /// prior commands. The caller is responsible for ending the command list
    /// and immediately submitting it to a graphics queue. The command list must
    /// be completely executed before calling `libra_d3d12_filter_chain_frame`
    PFN_libra_d3d12_filter_chain_create_deferred d3d12_filter_chain_create_deferred;

    /// Draw a frame with the given parameters for the given filter chain.
    ///
    /// ## Safety
    /// - `chain` may be null, invalid, but not uninitialized. If `chain` is
    /// null or invalid, this
    ///    function will return an error.
    /// - `mvp` may be null, or if it is not null, must be an aligned pointer to
    /// 16 consecutive `float`
    ///    values for the model view projection matrix.
    /// - `opt` may be null, or if it is not null, must be an aligned pointer to
    /// a valid `frame_d3d12_opt_t`
    ///    struct.
    PFN_libra_d3d12_filter_chain_frame d3d12_filter_chain_frame;

    /// Free a D3D11 filter chain.
    ///
    /// The resulting value in `chain` then becomes null.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d12_filter_chain_t`.
    PFN_libra_d3d12_filter_chain_free d3d12_filter_chain_free;

    /// Gets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d12_filter_chain_t`.
    PFN_libra_d3d12_filter_chain_get_active_pass_count
        d3d12_filter_chain_get_active_pass_count;

    /// Sets the number of active passes for this chain.
    ///
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d12_filter_chain_t`.
    PFN_libra_d3d12_filter_chain_set_active_pass_count
        d3d12_filter_chain_set_active_pass_count;

    /// Gets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d12_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_d3d12_filter_chain_get_param d3d12_filter_chain_get_param;

    /// Sets a parameter for the filter chain.
    ///
    /// If the parameter does not exist, returns an error.
    /// ## Safety
    /// - `chain` must be either null or a valid and aligned pointer to an
    /// initialized `libra_d3d12_filter_chain_t`.
    /// - `param_name` must be either null or a null terminated string.
    PFN_libra_d3d12_filter_chain_set_param d3d12_filter_chain_set_param;
#endif

    /// Helper flag for if the librashader instance was loaded.
    ///
    /// This flag is not indicative of whether any functions were loaded
    /// properly or not. The flag is true immediately after the instance
    /// was created with librashader_load_instance if and only if:
    /// 
    /// 1. A librashader library was found in the search path.
    /// 2. The ABI version of the librashader library in the search path is compatible.
    /// 
    /// This flag can only be relied upon when checked immediately after
    /// librashader_load_instance as there is no protection against mutating
    /// this flag.
    /// 
    /// Regardless of the state of this flag, a librashader instance created
    /// with librashader_load_instance is always safe to call. An instance
    /// that fails to load is still valid to call as long as safety invariants
    /// are maintained. However, an unloaded function will be a no-op.
    bool instance_loaded;
} libra_instance_t;



libra_instance_t __librashader_make_null_instance() {
    return libra_instance_t {
        .instance_abi_version = __librashader__noop_instance_abi_version,
        .instance_api_version = __librashader__noop_instance_api_version,

        .preset_create = __librashader__noop_preset_create,
        .preset_free = __librashader__noop_preset_free,
        .preset_set_param = __librashader__noop_preset_set_param,
        .preset_get_param = __librashader__noop_preset_get_param,
        .preset_print = __librashader__noop_preset_print,
        .preset_get_runtime_params =
            __librashader__noop_preset_get_runtime_params,
        .preset_free_runtime_params =
            __librashader__noop_preset_free_runtime_params,

        .error_errno = __librashader__noop_error_errno,
        .error_print = __librashader__noop_error_print,
        .error_free = __librashader__noop_error_free,
        .error_write = __librashader__noop_error_write,
        .error_free_string = __librashader__noop_error_free_string,

#if defined(LIBRA_RUNTIME_OPENGL)
        .gl_init_context = __librashader__noop_gl_init_context,
        .gl_filter_chain_create = __librashader__noop_gl_filter_chain_create,
        .gl_filter_chain_frame = __librashader__noop_gl_filter_chain_frame,
        .gl_filter_chain_free = __librashader__noop_gl_filter_chain_free,
        .gl_filter_chain_get_active_pass_count =
            __librashader__noop_gl_filter_chain_get_active_pass_count,
        .gl_filter_chain_set_active_pass_count =
            __librashader__noop_gl_filter_chain_set_active_pass_count,
        .gl_filter_chain_get_param =
            __librashader__noop_gl_filter_chain_get_param,
        .gl_filter_chain_set_param =
            __librashader__noop_gl_filter_chain_set_param,
#endif

#if defined(LIBRA_RUNTIME_VULKAN)
        .vk_filter_chain_create = __librashader__noop_vk_filter_chain_create,
        .vk_filter_chain_create_deferred = __librashader__noop_vk_filter_chain_create_deferred,
        .vk_filter_chain_frame = __librashader__noop_vk_filter_chain_frame,
        .vk_filter_chain_free = __librashader__noop_vk_filter_chain_free,
        .vk_filter_chain_get_active_pass_count =
            __librashader__noop_vk_filter_chain_get_active_pass_count,
        .vk_filter_chain_set_active_pass_count =
            __librashader__noop_vk_filter_chain_set_active_pass_count,
        .vk_filter_chain_get_param =
            __librashader__noop_vk_filter_chain_get_param,
        .vk_filter_chain_set_param =
            __librashader__noop_vk_filter_chain_set_param,
#endif

#if defined(LIBRA_RUNTIME_D3D11)
        .d3d11_filter_chain_create =
            __librashader__noop_d3d11_filter_chain_create,
        .d3d11_filter_chain_create_deferred =
            __librashader__noop_d3d11_filter_chain_create_deferred,
        .d3d11_filter_chain_frame =
            __librashader__noop_d3d11_filter_chain_frame,
        .d3d11_filter_chain_free = __librashader__noop_d3d11_filter_chain_free,
        .d3d11_filter_chain_get_active_pass_count =
            __librashader__noop_d3d11_filter_chain_get_active_pass_count,
        .d3d11_filter_chain_set_active_pass_count =
            __librashader__noop_d3d11_filter_chain_set_active_pass_count,
        .d3d11_filter_chain_get_param =
            __librashader__noop_d3d11_filter_chain_get_param,
        .d3d11_filter_chain_set_param =
            __librashader__noop_d3d11_filter_chain_set_param,
#endif

#if defined(LIBRA_RUNTIME_D3D12)
        .d3d12_filter_chain_create =
            __librashader__noop_d3d12_filter_chain_create,
        .d3d12_filter_chain_create_deferred =
            __librashader__noop_d3d12_filter_chain_create_deferred,
        .d3d12_filter_chain_frame =
            __librashader__noop_d3d12_filter_chain_frame,
        .d3d12_filter_chain_free = __librashader__noop_d3d12_filter_chain_free,
        .d3d12_filter_chain_get_active_pass_count =
            __librashader__noop_d3d12_filter_chain_get_active_pass_count,
        .d3d12_filter_chain_set_active_pass_count =
            __librashader__noop_d3d12_filter_chain_set_active_pass_count,
        .d3d12_filter_chain_get_param =
            __librashader__noop_d3d12_filter_chain_get_param,
        .d3d12_filter_chain_set_param =
            __librashader__noop_d3d12_filter_chain_set_param,
#endif
        .instance_loaded = false,
    };
}

/// Load an instance of librashader in the OS-dependent search path of the
/// current directory.
///
/// `librashader_load_instance` loads from `librashader.dll` on Windows,
/// or `librashader.so` on Linux.
///
/// If no librashader implementation is found, the returned `libra_instance_t`
/// will have all function pointers set to no-op functions.
///
/// If any symbol fails to load, the function will be set to a no-op function.
///
/// \return An `libra_instance_t` struct with loaded function pointers.
libra_instance_t librashader_load_instance();

#if defined(_WIN32) || defined(__linux__)
libra_instance_t librashader_load_instance() {
    _LIBRASHADER_IMPL_HANDLE librashader = _LIBRASHADER_LOAD;
    libra_instance_t instance = __librashader_make_null_instance();
    if (librashader == 0) {
        return instance;
    }

    _LIBRASHADER_ASSIGN(librashader, instance, instance_abi_version);
    _LIBRASHADER_ASSIGN(librashader, instance, instance_api_version);

    // Ensure ABI matches.
    if (instance.instance_abi_version() != LIBRASHADER_CURRENT_ABI) {
        return instance;
    }

    _LIBRASHADER_ASSIGN(librashader, instance, preset_create);
    _LIBRASHADER_ASSIGN(librashader, instance, preset_free);
    _LIBRASHADER_ASSIGN(librashader, instance, preset_set_param);
    _LIBRASHADER_ASSIGN(librashader, instance, preset_get_param);
    _LIBRASHADER_ASSIGN(librashader, instance, preset_print);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                preset_get_runtime_params);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                preset_free_runtime_params);

    _LIBRASHADER_ASSIGN(librashader, instance, error_errno);
    _LIBRASHADER_ASSIGN(librashader, instance, error_print);
    _LIBRASHADER_ASSIGN(librashader, instance, error_free);
    _LIBRASHADER_ASSIGN(librashader, instance, error_write);
    _LIBRASHADER_ASSIGN(librashader, instance, error_free_string);

#if defined(LIBRA_RUNTIME_OPENGL)
    _LIBRASHADER_ASSIGN(librashader, instance, gl_init_context);
    _LIBRASHADER_ASSIGN(librashader, instance, gl_filter_chain_create);
    _LIBRASHADER_ASSIGN(librashader, instance, gl_filter_chain_frame);
    _LIBRASHADER_ASSIGN(librashader, instance, gl_filter_chain_free);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                gl_filter_chain_get_param);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                gl_filter_chain_set_param);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                gl_filter_chain_get_active_pass_count);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                gl_filter_chain_set_active_pass_count);

#endif

#if defined(LIBRA_RUNTIME_VULKAN)
    _LIBRASHADER_ASSIGN(librashader, instance, vk_filter_chain_create);
    _LIBRASHADER_ASSIGN(librashader, instance, vk_filter_chain_create_deferred);
    _LIBRASHADER_ASSIGN(librashader, instance, vk_filter_chain_frame);
    _LIBRASHADER_ASSIGN(librashader, instance, vk_filter_chain_free);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                vk_filter_chain_get_param);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                vk_filter_chain_set_param);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                vk_filter_chain_get_active_pass_count);
    _LIBRASHADER_ASSIGN(librashader, instance,
                                vk_filter_chain_set_active_pass_count);
#endif

#if defined(_WIN32) && defined(LIBRA_RUNTIME_D3D11)
    _LIBRASHADER_ASSIGN(librashader, instance, d3d11_filter_chain_create);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d11_filter_chain_create_deferred);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d11_filter_chain_frame);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d11_filter_chain_free);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d11_filter_chain_get_param);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d11_filter_chain_set_param);
    _LIBRASHADER_ASSIGN(librashader, instance,
                        d3d11_filter_chain_get_active_pass_count);
    _LIBRASHADER_ASSIGN(librashader, instance,
                        d3d11_filter_chain_set_active_pass_count);
#endif

#if defined(_WIN32) && defined(LIBRA_RUNTIME_D3D12)
    _LIBRASHADER_ASSIGN(librashader, instance, d3d12_filter_chain_create);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d12_filter_chain_create_deferred);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d12_filter_chain_frame);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d12_filter_chain_free);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d12_filter_chain_get_param);
    _LIBRASHADER_ASSIGN(librashader, instance, d3d12_filter_chain_set_param);
    _LIBRASHADER_ASSIGN(librashader, instance,
                        d3d12_filter_chain_get_active_pass_count);
    _LIBRASHADER_ASSIGN(librashader, instance,
                        d3d12_filter_chain_set_active_pass_count);
#endif
    instance.instance_loaded = true;
    return instance;
}
#else
libra_instance_t librashader_load_instance() {
    return __librashader_make_null_instance();
}
#endif
#endif
