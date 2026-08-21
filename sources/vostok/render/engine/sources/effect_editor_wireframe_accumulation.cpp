#include "pch.h"
#include "effect_editor_wireframe_accumulation.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_editor_wireframe_accumulation::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;

	u32 vertex_input_type = 0;

	float3 wireframe_colors[u32(num_vertex_input_types)];

	for (u32 i = 0; i < u32(num_vertex_input_types); i++)
	{
		wireframe_colors[i] = float3(0.0f, 0.5f, 0.5f);
	}

 	wireframe_colors[skeletal_4_bones_mesh_vertex_input_type] =
 	wireframe_colors[skeletal_3_bones_mesh_vertex_input_type] =
 	wireframe_colors[skeletal_2_bones_mesh_vertex_input_type] =
 	wireframe_colors[skeletal_1_bones_mesh_vertex_input_type] = float3(0.0f, 0.25f, 0.75f);

	wireframe_colors[particle_vertex_input_type] =
 	wireframe_colors[particle_subuv_vertex_input_type] =
	wireframe_colors[particle_beamtrail_vertex_input_type] = float3(1.0f, 0.25f, 0.75f);

	wireframe_colors[grassmesh_vertex_input_type] = float3(0.1f, 0.75f, 0.1f);

	wireframe_colors[wires_vertex_input_type] = float3(0.7f, 0.5f, 0.1f);

	if (config.value_exists("vertex_input_type"))
		vertex_input_type = u32((enum_vertex_input_type)config["vertex_input_type"]);

	compile_begin("vertex_base", "editor_wireframe_accumulation", compiler, &configuration, config);
		compiler.set_depth(true, true);
		compiler.set_fill_mode(D3D_FILL_WIREFRAME);
		compiler.set_constant("wireframe_color", wireframe_colors[vertex_input_type]);
	compile_end(compiler);

	compile_begin("vertex_base", "editor_wireframe_accumulation", compiler, &configuration, config);
		compiler.set_depth(true, true);
		compiler.set_fill_mode(D3D_FILL_WIREFRAME);
		compiler.set_cull_mode(D3D_CULL_NONE);
		compiler.set_constant("wireframe_color", wireframe_colors[vertex_input_type]);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
