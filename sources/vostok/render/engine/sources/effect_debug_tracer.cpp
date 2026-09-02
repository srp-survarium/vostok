// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_debug_tracer.h"
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_debug_tracer::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	shader_configuration configuration;

	compile_begin("vertex_base", "tracer", compiler, &configuration, config);
		compiler.set_depth(true, true);
		compiler.set_cull_mode(D3D_CULL_NONE);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
