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
	// 4 target lines are likely retail-compiled-out source.
	for (u32 pass_index = 0; pass_index < 2; pass_index++)
	{
		shader_configuration configuration;

		configuration.use_diffuse_texture = true;
		configuration.use_detail_texture = custom_config.value_exists("use_tdetail") && bool(custom_config["use_tdetail"]);
		configuration.use_normal_texture = custom_config.value_exists("use_nmap") && bool(custom_config["use_nmap"]);

		configuration.use_specular_intensity_texture = custom_config.value_exists("use_specular_intensity_map") && bool(custom_config["use_specular_intensity_map"]);

		configuration.terrain_blend_mode = custom_config.value_exists("terrain_blend_mode") ? u32(custom_config["terrain_blend_mode"]) : 0;

		bool const use_height_map_0 = custom_config.value_exists("use_height_map_0") && bool(custom_config["use_height_map_0"]);
		bool const use_height_map_1 = custom_config.value_exists("use_height_map_1") && bool(custom_config["use_height_map_1"]);
		bool const use_height_map_2 = custom_config.value_exists("use_height_map_2") && bool(custom_config["use_height_map_2"]);
		bool const use_height_map_3 = custom_config.value_exists("use_height_map_3") && bool(custom_config["use_height_map_3"]);

		configuration.use_parallax = use_height_map_0 || use_height_map_1 || use_height_map_2 || use_height_map_3;

		if (use_height_map_1) configuration.used_terrain_height_mask |= 4;
		if (use_height_map_2) configuration.used_terrain_height_mask |= 2;
		if (use_height_map_3) configuration.used_terrain_height_mask |= 1;
		if (custom_config.value_exists("num_used_terrain_layers"))
			configuration.num_used_terrain_layers = math::clamp_r(u32(custom_config["num_used_terrain_layers"]), 1u, 4u);

		compile_begin("vertex_base", "terrain_gbuffer_pass", compiler, &configuration, custom_config);

		compiler.set_stencil(
			true,
			all_geometry_type + static_geometry_type,
			0xff, 0xff,
			D3D_COMPARISON_ALWAYS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_REPLACE, D3D_STENCIL_OP_KEEP
		);

		compiler.set_depth(true, true);

		compiler.set_texture("texture_mask", pcstr(custom_config["texture_mask"]), 0, false, u32(-1));

		float4 tiles = float4(
			float(custom_config["constant_tile_0"]),
			float(custom_config["constant_tile_1"]),
			float(custom_config["constant_tile_2"]),
			float(custom_config["constant_tile_3"])
		);

		if (configuration.use_normal_texture)
		{
			compiler.set_texture("texture_normal_0", pcstr(custom_config["texture_normal_0"]), 0, false, u32(-1));
			compiler.set_texture("texture_normal_1", pcstr(custom_config["texture_normal_1"]), 0, false, u32(-1));
			compiler.set_texture("texture_normal_2", pcstr(custom_config["texture_normal_2"]), 0, false, u32(-1));
			compiler.set_texture("texture_normal_3", pcstr(custom_config["texture_normal_3"]), 0, false, u32(-1));
			compiler.set_constant("constant_tiles", tiles);
		}

		compiler.set_texture("texture_specular_intensity", pcstr(custom_config["texture_specular_intensity"]), 0, false, u32(-1));

		if (
			configuration.use_parallax)
		{
			compiler.set_texture("texture_height_0", pcstr(custom_config["texture_height_0"]), 0, false, u32(-1));
			compiler.set_texture("texture_height_1", pcstr(custom_config["texture_height_1"]), 0, false, u32(-1));
			compiler.set_texture("texture_height_2", pcstr(custom_config["texture_height_2"]), 0, false, u32(-1));
			compiler.set_texture("texture_height_3", pcstr(custom_config["texture_height_3"]), 0, false, u32(-1));
			compiler.set_constant("constant_tiles", tiles);
		}

		compiler.set_texture("texture_diffuse_0", pcstr(custom_config["texture_diffuse_0"]), 0, false, u32(-1));
		compiler.set_texture("texture_diffuse_1", pcstr(custom_config["texture_diffuse_1"]), 0, false, u32(-1));
		compiler.set_texture("texture_diffuse_2", pcstr(custom_config["texture_diffuse_2"]), 0, false, u32(-1));
		compiler.set_texture("texture_diffuse_3", pcstr(custom_config["texture_diffuse_3"]), 0, false, u32(-1));

		compiler.set_constant("constant_tiles", tiles);

		compiler.set_texture("t_normal", "$user$normal", 0, false, u32(-1));
		compiler.set_texture("t_decals_normal", "$user$decals_normal", 0, false, u32(-1));

		if (configuration.use_detail_texture)
		{
			compiler.set_texture("texture_detail_0", pcstr(custom_config["texture_detail_0"]), 0, false, u32(-1));
			compiler.set_texture("texture_detail_1", pcstr(custom_config["texture_detail_1"]), 0, false, u32(-1));

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
		compile_end(compiler);
	}

	// fill rsm backend
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base_lpv", "fill_reflective_shadow_map_backend", compiler, &local_configuration, custom_config);
		compile_end(compiler);
	}

	// fill vertex color
	{
		shader_configuration local_configuration;
		local_configuration.use_detail_texture = custom_config.value_exists("use_tdetail") && bool(custom_config["use_tdetail"]);

		compile_begin("fill_vertex_color", "fill_vertex_color", compiler, &local_configuration, custom_config);

		compiler.set_depth(false, false);
		compiler.set_stencil(false);

		if (local_configuration.use_detail_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_detail_0"]), 0, false, u32(-1));

		compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));

		compile_end(compiler);
	}

	// fill rsm
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base", "fill_reflective_shadow_map", compiler, &local_configuration, custom_config);
		local_configuration.use_detail_texture = custom_config.value_exists("use_tdetail") && bool(custom_config["use_tdetail"]);
		if (local_configuration.use_detail_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_detail_0"]), 0, false, u32(-1));
		compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));
		compile_end(compiler);
	}

	// emissive
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base", "gbuffer_emissive_pass", compiler, &local_configuration, custom_config);
		compiler.set_depth(true, false);
		compiler.set_stencil(false);

		float4 solid_emission_color;

		if (local_configuration.use_emissive != 0)
		{
			float multiplier = float(custom_config["constant_emissive_multiplier"]);
			solid_emission_color = float4(custom_config["constant_emissive"]);
			solid_emission_color.xyz() *= multiplier;
			compiler.set_constant("solid_emission_color", solid_emission_color);
		}

		compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);

		if (local_configuration.use_emissive == 2)
		{
			compiler.set_texture("t_emission", pcstr(custom_config["texture_emissive"]), 0, false, u32(-1));
		}

		compile_end(compiler);
	}

	// motion vectors
	{

		shader_configuration local_configuration;

		local_configuration.use_alpha_test = custom_config.value_exists("use_alpha_test") ? bool(custom_config["use_alpha_test"]) : false;
		local_configuration.use_diffuse_texture = custom_config.value_exists("use_tdiffuse") ? bool(custom_config["use_tdiffuse"]) : false;

		compile_begin("vertex_base", "motion_vectors_accumulation", compiler, &local_configuration, custom_config);
		compiler.set_depth(true, false);

		float alpha_ref = 0.25f;

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, false, u32(-1));

		if (local_configuration.use_alpha_test && custom_config.value_exists("alpha_ref"))
			alpha_ref = float(custom_config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		compiler.set_cull_mode(D3D_CULL_NONE);
		compile_end(compiler);
	}

	// subsurface scattering
	{

		shader_configuration local_configuration;

		local_configuration.use_alpha_test = custom_config.value_exists("use_alpha_test") ? bool(custom_config["use_alpha_test"]) : false;
		local_configuration.use_diffuse_texture = custom_config.value_exists("use_tdiffuse") ? bool(custom_config["use_tdiffuse"]) : false;

		compile_begin("vertex_base", "subsurface_scattering", compiler, &local_configuration, custom_config);
		compiler.set_depth(true, false);

		float alpha_ref = 0.25f;

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, false, u32(-1));

		if (local_configuration.use_alpha_test && custom_config.value_exists("alpha_ref"))
			alpha_ref = float(custom_config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		compiler.set_texture("t_diffuse_lighting", "$user$accum_diffuse", 0, false, u32(-1));
		compiler.set_texture("t_specular_lighting", "$user$accum_specular", 0, false, u32(-1));

		compiler.set_cull_mode(D3D_CULL_NONE);
		compile_end(compiler);
	}
	// z only
	{
		shader_configuration local_configuration;

		compile_begin("vertex_base", "z_only", compiler, &local_configuration, custom_config);
		compiler.set_depth(true, true);
		compiler.color_write_enable(D3D_COLOR_WRITE_ENABLE_NONE);
		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
