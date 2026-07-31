#include "pch.h"
#include "effect_gbuffer_nomaterial_materials.h"
#include "geometry_type.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_gbuffer_nomaterial_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7bf140]
	for (u32 pass_index = 0; pass_index < 2; pass_index++)
	{
		shader_configuration configuration;

		if (pass_index == 0)
		{
			configuration.use_diffuse_texture	= false;
			compile_begin("vertex_base", NULL, "geometry_pre_pass_nomaterial", compiler, &configuration, config);
		}
		else
		{
			configuration.use_diffuse_texture	= true;
			compile_begin("vertex_base", NULL, "geometry_material_pass_nomaterial", compiler, &configuration, config);
		}

		if (pass_index == 0)
		{
			compiler.set_stencil(
				true,
				all_geometry_type + static_geometry_type,
				0xff,
				0xff,
				D3D_COMPARISON_ALWAYS,
				D3D_STENCIL_OP_KEEP,
				D3D_STENCIL_OP_REPLACE,
				D3D_STENCIL_OP_KEEP
			);
		}
		else
		{
			compiler.set_stencil(false);
			compiler.set_depth(true, false);
			compiler.set_texture("t_default_texture", "no_texture", 0, true, 0);
			compiler.set_cull_mode(D3D_CULL_NONE);
		}
		compile_end(compiler);
	}
	// fill rsm
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base", "fill_reflective_shadow_map", compiler, &local_configuration, config);
			compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));
			compiler.set_texture("t_base", "no_texture", 0, true, 0);
		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
