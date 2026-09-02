// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_editor_show_miplevel.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
namespace vostok {
namespace render {

void effect_editor_show_miplevel::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;

	compile_begin("vertex_base", "editor_texture_mip_level", compiler, &configuration, config);
		compiler.set_depth(true, true);
		compiler.set_cull_mode(D3D_CULL_NONE);
		compiler.set_texture("t_albedo_color", r2_rt_albedo, 0, false, u32(-1));
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
