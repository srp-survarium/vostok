// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_editor_show_batched_geometry.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/facade/vertex_input_type.h>
namespace vostok {
namespace render {

void effect_editor_show_batched_geometry::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;

	compile_begin("vertex_base_lpv", "editor_show_lpv_geometry", compiler, &configuration, config);
	compile_end(compiler);

	compile_begin("vertex_base_shadow_batched", "editor_show_shadow_geometry", compiler, &configuration, config);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
