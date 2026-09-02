// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_particle_selection.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/custom_config_value.h>
namespace vostok {
namespace render {

void effect_particle_selection::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	enum_vertex_input_type vertex_types[] = {particle_vertex_input_type,
											 particle_subuv_vertex_input_type,
											 particle_beamtrail_vertex_input_type};

	for (u32 index = 0; index < 3; index++)
	{
		shader_configuration configuration;
		configuration.vertex_input_type = vertex_types[index];

		compile_begin("vertex_base", "particle_selected", compiler, &configuration, custom_config);
			compiler.set_depth( true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_alpha_blend(true,D3D_BLEND_ONE,D3D_BLEND_ONE);
			compiler.set_fill_mode(D3D_FILL_WIREFRAME);
		compile_end(compiler);
	}

}

} // namespace render
} // namespace vostok
