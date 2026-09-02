// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_speedtree_selection.h"
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>
namespace vostok {
namespace render {

void effect_speedtree_selection::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	shader_configuration configuration;

	configuration.vertex_input_type	= u32(null_vertex_input_type);

	compiler.begin_technique(/*branch*/);
		compiler.begin_pass("vertex_base", NULL, "speedtree_selection", configuration, NULL);
			compiler.set_depth( false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_alpha_blend(true,D3D_BLEND_ONE,D3D_BLEND_ONE);
			compiler.set_fill_mode(D3D_FILL_WIREFRAME);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
