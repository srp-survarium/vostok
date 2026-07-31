#include "pch.h"
#include "effect_gstage_terrain_materials.h"
#include "geometry_type.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_gstage_terrain_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7b7e50]
	shader_configuration configuration;

	configuration.use_diffuse_texture		=	true;
	configuration.use_detail_texture		=	custom_config.value_exists("use_tdetail") && bool(custom_config["use_tdetail"]);
	configuration.use_normal_texture		=	custom_config.value_exists("use_nmap") && bool(custom_config["use_nmap"]);

	for (u32 pass_index = 0; pass_index < 2; pass_index++)
	{
			if (pass_index == 0)
				compile_begin("vertex_base", NULL, "terrain_pre_pass", compiler, &configuration, custom_config);
			else
			{
				configuration.use_anisotropic_direction_texture = false;
				compile_begin("vertex_base", NULL, "terrain_material_pass", compiler, &configuration, custom_config);
			}

			if (pass_index == 0)
				compiler.set_stencil(
					true,
					all_geometry_type + static_geometry_type,
					0xff, 0xff,
					D3D_COMPARISON_ALWAYS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_REPLACE, D3D_STENCIL_OP_KEEP
				);
			else
				compiler.set_stencil(false);

			compiler.set_texture("texture_mask", pcstr(custom_config["texture_mask"]), 0, true, 0);

			float4 tiles	=	float4(
									float(custom_config["constant_tile_0"]),
									float(custom_config["constant_tile_1"]),
									float(custom_config["constant_tile_2"]),
									float(custom_config["constant_tile_3"])
								);

			if (pass_index == 0)
			{
				if (configuration.use_normal_texture)
				{
					compiler.set_texture("texture_normal_0", pcstr(custom_config["texture_normal_0"]), 0, true, 0);
					compiler.set_texture("texture_normal_1", pcstr(custom_config["texture_normal_1"]), 0, true, 0);
					compiler.set_texture("texture_normal_2", pcstr(custom_config["texture_normal_2"]), 0, true, 0);
					compiler.set_texture("texture_normal_3", pcstr(custom_config["texture_normal_3"]), 0, true, 0);
					compiler.set_constant("constant_tiles",  tiles);
				}
			}

			if (pass_index == 1)
			{
				compiler.set_depth(true, false);

				compiler.set_texture("t_accumulator_dif", r2_rt_accum_diffuse, 0, false, 0);
				compiler.set_texture("t_accumulator_spec", r2_rt_accum_specular, 0, false, 0);
				//compiler.set_texture("t_ssao_accumulator",	r2_rt_ssao_accumulator);
				compiler.set_texture("t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0);

				compiler.set_texture("texture_diffuse_0", pcstr(custom_config["texture_diffuse_0"]), 0, true, 0);
				compiler.set_texture("texture_diffuse_1", pcstr(custom_config["texture_diffuse_1"]), 0, true, 0);
				compiler.set_texture("texture_diffuse_2", pcstr(custom_config["texture_diffuse_2"]), 0, true, 0);
				compiler.set_texture("texture_diffuse_3", pcstr(custom_config["texture_diffuse_3"]), 0, true, 0);

				compiler.set_constant("constant_tiles",  tiles);

				if (configuration.use_detail_texture)
				{
					compiler.set_texture("texture_detail_0", pcstr(custom_config["texture_detail_0"]), 0, true, 0);
					compiler.set_texture("texture_detail_1", pcstr(custom_config["texture_detail_1"]), 0, true, 0);

					compiler.set_constant(
						"constant_detail_tiles",
						float4(
							float(custom_config["constant_detail_tile_0"]),
							float(custom_config["constant_detail_tile_1"]),
							0.0f,
							0.0f
						)
					);
				}
			}

		compile_end(compiler);
	}

	// fill rsm
	{
		shader_configuration local_configuration;
		local_configuration.use_diffuse_texture		= false;//bool(custom_config["use_tdiffuse"]);

		compile_begin("vertex_base", "fill_reflective_shadow_map", compiler, &local_configuration, custom_config);

		if( local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, true, 0);

		//if (custom_config.value_exists("constant_diffuse"))
		//	compiler.set_constant("diffuse_color_parameter", float4(custom_config["constant_diffuse"]).xyz());
		//else
			compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));

		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
