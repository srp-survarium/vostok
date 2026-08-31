////////////////////////////////////////////////////////////////////////////
//	Created		: 14.03.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "effect_gstage_default_materials.h"
#include "geometry_type.h"
#include "shared_names.h"

#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

static command_line::key s_z_only_0( "z_only_0", "", "", "" );

static u32 get_stencil_mask( enum_vertex_input_type vit, bool )
{

	switch (vit)
	{
	case skeletal_4_bones_mesh_vertex_input_type:
	case skeletal_3_bones_mesh_vertex_input_type:
	case skeletal_2_bones_mesh_vertex_input_type:
	case skeletal_1_bones_mesh_vertex_input_type:
		return all_geometry_type + skeleton_geometry_type;
	case particle_vertex_input_type:
	case particle_subuv_vertex_input_type:
	case particle_beamtrail_vertex_input_type:
		return all_geometry_type + particle_geometry_type;
	default:
		return all_geometry_type + static_geometry_type;
	}
}

void effect_gstage_default_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	bool is_static_mesh = false;
	if (custom_config.value_exists("vertex_input_type"))
	{
		enum_vertex_input_type vertex_input_type = (enum_vertex_input_type)custom_config["vertex_input_type"];
		is_static_mesh = vertex_input_type != grassmesh_vertex_input_type;
	}

	u32 const debug_last_mips = is_static_mesh ? 5 : u32(-1);

	for (s32 lod_index = 0; lod_index < 2; lod_index++)
	{
		bool const first_lod = lod_index == 0;
		shader_configuration configuration;
		configuration.use_diffuse_texture = bool(custom_config["use_tdiffuse"]);
		configuration.use_normal_texture = first_lod && bool(custom_config["use_nmap"]);
		configuration.use_parallax = first_lod && bool(custom_config["use_parallax"]);
		configuration.use_alpha_test = bool(custom_config["use_alpha_test"]);
		configuration.use_detail_normal_texture = first_lod && custom_config.value_exists("use_detail_nmap") ? bool(custom_config["use_detail_nmap"]) : false;
		configuration.use_indirect_specular = first_lod && custom_config.value_exists("use_indirect_specular") ? bool(custom_config["use_indirect_specular"]) : false;
		configuration.use_bound_normals = custom_config.value_exists("use_bound_normals") ? bool(custom_config["use_bound_normals"]) : false;
		configuration.use_up_directional_normals = custom_config.value_exists("use_up_directional_normals") ? bool(custom_config["use_up_directional_normals"]) : false;
		configuration.use_anisotropic_direction_texture = first_lod && custom_config.value_exists("use_anisotropic_direction_texture") ? bool(custom_config["use_anisotropic_direction_texture"]) : false;
		configuration.use_detail_texture = custom_config.value_exists("use_tditail_diffuse") ? bool(custom_config["use_tditail_diffuse"]) : false;
		configuration.use_reflection = bool(custom_config["use_reflection"]) ? 1 : 0;
		configuration.use_reflection_mask = custom_config.value_exists("use_reflection_mask") ? bool(custom_config["use_reflection_mask"]) : false;
		configuration.use_sequence = first_lod && custom_config.value_exists("use_sequence") ? bool(custom_config["use_sequence"]) : false;
		configuration.alphablended_normal = first_lod && custom_config.value_exists("use_alphablended_normal") && bool(custom_config["use_alphablended_normal"]);
		configuration.use_diffuse_as_specular = first_lod && custom_config.value_exists("use_diffuse_as_specular") && bool(custom_config["use_diffuse_as_specular"]);
		configuration.use_vertex_blended_mask = first_lod && custom_config.value_exists("use_vertex_blended_mask") && bool(custom_config["use_vertex_blended_mask"]);
		configuration.use_vertex_blended_diffuse = first_lod && custom_config.value_exists("use_vertex_blended_diffuse") && bool(custom_config["use_vertex_blended_diffuse"]);
		configuration.use_vertex_blended_normal = first_lod && custom_config.value_exists("use_vertex_blended_normal") && bool(custom_config["use_vertex_blended_normal"]);
		configuration.use_vertex_blended_specular = first_lod && custom_config.value_exists("use_vertex_blended_specular") && bool(custom_config["use_vertex_blended_specular"]);
		configuration.use_grass_fresnel_effect = first_lod && custom_config.value_exists("use_grass_fresnel_effect") && bool(custom_config["use_grass_fresnel_effect"]);
		configuration.use_specular_intensity_texture = custom_config.value_exists("use_tspecular_intensity") ? bool(custom_config["use_tspecular_intensity"]) : false;
		configuration.use_fresnel_texture = custom_config.value_exists("use_tfresnel") ? bool(custom_config["use_tfresnel"]) : false;
		configuration.use_roughness_texture = custom_config.value_exists("use_troughness") ? bool(custom_config["use_troughness"]) : false;
		configuration.use_translucency_texture = bool(custom_config["use_ttranslucency"]);
		configuration.lod_index = lod_index;
		configuration.use_specular_lighting = true;
		configuration.use_diffuse_masked_color = custom_config.value_exists("use_masked_diffuse_color") && bool(custom_config["use_masked_diffuse_color"]);

		configuration.use_olta = custom_config.value_exists("use_olta") && bool(custom_config["use_olta"]);

		configuration.use_fuzziness = custom_config.value_exists("use_fuzziness") && bool(custom_config["use_fuzziness"]);

		configuration.use_diffuse_masked_color_by_hue = configuration.use_diffuse_masked_color && (!custom_config.value_exists("use_constant_mask_color") || !bool(custom_config["use_constant_mask_color"]));

		configuration.use_normal_waves = custom_config.value_exists("use_normal_waves") && bool(custom_config["use_normal_waves"]);

		configuration.use_uv_scrolling = custom_config.value_exists("use_uv_scrolling") && bool(custom_config["use_uv_scrolling"]);

		if (custom_config.value_exists("use_variation_mask") && custom_config.value_exists("texture_variation_mask"))

			configuration.use_variation_mask = bool(custom_config["use_variation_mask"]) && !strings::equal(pcstr(custom_config["texture_variation_mask"]), "");
		if (custom_config.value_exists("wind_motion"))
		{
			configuration.wind_motion = s32(custom_config["wind_motion"]);
		}
		if (first_lod && bool(custom_config["use_reflection"]))
		{
			configuration.use_reflection = 1;

			if (bool(custom_config["use_reflection_diffuse"]))
				configuration.use_reflection = 2;
		}

		configuration.is_anisotropic_material = custom_config.value_exists("is_anisotropic_material") ? bool(custom_config["is_anisotropic_material"]) : false;

		compile_begin("vertex_base", "gbuffer_pass", compiler, &configuration, custom_config);

		compiler.set_stencil(
			true,
			configuration.use_translucency_texture ? all_geometry_type + translucency_geometry_type
				: get_stencil_mask((enum_vertex_input_type)configuration.vertex_input_type, first_lod),
			0xff,
			0xff,
			D3D_COMPARISON_ALWAYS,
			D3D_STENCIL_OP_KEEP,
			D3D_STENCIL_OP_REPLACE,
			D3D_STENCIL_OP_KEEP
		);

		compiler.set_depth(true, s_z_only_0);
		if (custom_config.value_exists("constant_tile_u") && custom_config.value_exists("constant_tile_v"))
			compiler.set_constant("constant_tile_uv", float2(float(custom_config["constant_tile_u"]), float(custom_config["constant_tile_v"])));
		else
			compiler.set_constant("constant_tile_uv", float2(1.0f, 1.0f));

		float alpha_ref = 0.25f;

		if (configuration.use_alpha_test && custom_config.value_exists("alpha_ref"))
			alpha_ref = float(custom_config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);
		if (configuration.use_fuzziness &&
			custom_config.value_exists("fuzziness_saturation") &&
			custom_config.value_exists("fuzziness_multiplier") &&
			custom_config.value_exists("fuzziness_power"))
			compiler.set_constant(
				"fuzziness_parameters",
				float3(
					float(custom_config["fuzziness_saturation"]),
					float(custom_config["fuzziness_multiplier"]),
					float(custom_config["fuzziness_power"])
				)
			);
		compiler.set_texture("t_grass_motion_mask", "engine/test_grass_motion_mask", 0, false, u32(-1));

		if (configuration.use_olta &&
			custom_config.value_exists("texture_transparency"))
			compiler.set_texture("t_transparency", pcstr(custom_config["texture_transparency"]), 0, false, u32(-1));

		if (configuration.use_diffuse_masked_color)
		{
			compiler.set_texture("t_diffuse_color_mask", shared_string(pcstr(custom_config["use_diffuse_color_mask_texture"])), is_static_mesh, debug_last_mips);

			if (configuration.use_diffuse_masked_color_by_hue)
			{
				float const angle = float(custom_config["constant_diffuse_masked_hue"]) * math::pi;

				math::sine_cosine sc(angle);
				float3 cvec(0.0f, 0.0f, 1.0f);

				cvec = math::normalize_safe(float3(sc.cosine, 1.0f - sc.cosine - sc.sine, 1.0f - sc.cosine + sc.sine));

				compiler.set_constant("hue_matrix_component_x", cvec);
				compiler.set_constant("hue_matrix_component_y", float3(cvec.z, cvec.x, cvec.y));
				compiler.set_constant("hue_matrix_component_z", float3(cvec.y, cvec.z, cvec.x));
			}
			else
			{
				if (custom_config.value_exists("constant_diffuse_color_mask_color"))
					compiler.set_constant("constant_diffuse_mask_color", float4(custom_config["constant_diffuse_color_mask_color"]).xyz());
				else
					compiler.set_constant("constant_diffuse_mask_color", float3(0.0f, 0.0f, 0.0f));
			}
		}
		if (configuration.use_normal_waves && custom_config.value_exists("normal_waves_moving_speed") && custom_config.value_exists("normal_waves_intensity") && custom_config.value_exists("normal_waves_tile"))
		{
			compiler.set_texture("t_normal_waves", shared_string(pcstr(custom_config["normal_waves_texture"])), is_static_mesh, debug_last_mips);
			compiler.set_constant("normal_waves_parameters", float3(float(custom_config["normal_waves_moving_speed"]), float(custom_config["normal_waves_intensity"]), float(custom_config["normal_waves_tile"])));
		}

		compiler.set_constant("smoothness_multiplier", 1.0f);
		if (configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, is_static_mesh, debug_last_mips);

		if (configuration.alphablended_diffuse)
		{
			if (custom_config.value_exists("texture_alphablended_diffuse"))
				compiler.set_texture("t_alphablended_diffuse", shared_string(pcstr(custom_config["texture_alphablended_diffuse"])), is_static_mesh, debug_last_mips);

			if (configuration.alphablended_normal && custom_config.value_exists("texture_alphablended_normal"))
				compiler.set_texture("t_alphablended_normal", shared_string(pcstr(custom_config["texture_alphablended_normal"])), is_static_mesh, debug_last_mips);
		}

		if (bool(custom_config["use_reflection"]))
		{
			if (custom_config.value_exists("reflection_power"))
				compiler.set_constant("reflection_power", float(custom_config["reflection_power"]));
			else
				compiler.set_constant("reflection_power", 1.0f);

			if (custom_config.value_exists("min_reflection_angle"))
				compiler.set_constant("min_reflection_angle", math::deg2rad(float(custom_config["min_reflection_angle"])));
			else
				compiler.set_constant("min_reflection_angle", 0.0f);

			if (custom_config.value_exists("specular_color_multiplier"))
				compiler.set_constant("specular_color_multiplier", float4(custom_config["specular_color_multiplier"]));
			else
				compiler.set_constant("specular_color_multiplier", float4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (custom_config.value_exists("wind_motion"))
		{
			if (custom_config.value_exists("detailed_bend_parameters_frequency"))
				compiler.set_constant(
					"detailed_bending_parameters",
					float3(
						float(custom_config["detailed_bend_parameters_frequency"]),
						float(custom_config["detailed_bend_parameters_leaf_amplitude"]),
						float(custom_config["detailed_bend_parameters_branch_amplitude"])
					));
			compiler.set_constant("wind_scale", 1.0f);
		}

		if (configuration.use_vertex_blended_mask)
		{
			if (custom_config.value_exists("texture_vertex_blended_mask"))
				compiler.set_texture("t_vertex_blended_mask", shared_string(pcstr(custom_config["texture_vertex_blended_mask"])), is_static_mesh, debug_last_mips);
		}

		if (configuration.use_vertex_blended_diffuse || configuration.use_vertex_blended_normal)
			if (
				custom_config.value_exists("constant_vertex_blended_factor") &&
				custom_config.value_exists("constant_vertex_blended_falloff") &&
				custom_config.value_exists("constant_vertex_blended_tiling"))
			{
				compiler.set_constant(
					"vertex_blend_parameters",
					float3(
						float(custom_config["constant_vertex_blended_factor"]),
						float(custom_config["constant_vertex_blended_falloff"]),
						float(custom_config["constant_vertex_blended_tiling"])
					)
					);
			}
		if (configuration.use_vertex_blended_diffuse)
		{
			if (custom_config.value_exists("texture_vertex_blended_diffuse"))
				compiler.set_texture("t_vertex_blended_diffuse", shared_string(pcstr(custom_config["texture_vertex_blended_diffuse"])), is_static_mesh, debug_last_mips);
		}
		if (configuration.use_vertex_blended_normal)
		{
			if (custom_config.value_exists("texture_vertex_blended_normal"))
				compiler.set_texture("t_vertex_blended_normal", shared_string(pcstr(custom_config["texture_vertex_blended_normal"])), is_static_mesh, debug_last_mips);
		}

		float4 solid_color_specular = float4(custom_config["constant_diffuse"]);
		solid_color_specular.w = 0.0f;

		if (configuration.use_reflection)
		{
			pcstr texture_cubemap = pcstr(custom_config["texture_cubemap"]);

			compiler.set_texture("t_cubemap", strings::equal(texture_cubemap, "") ? "cubemap/reflect_blue" : texture_cubemap, 0, false, u32(-1));

			if (configuration.use_reflection_mask)
			{
				compiler.set_texture("t_cubemap_mask", pcstr(custom_config["texture_cubemap_mask"]), 0, false, u32(-1));
			}

			compiler.set_texture("t_frame_luminance", "$user$frame_luminance", 0, false, u32(-1));

			if (custom_config.value_exists("reflection_power"))
				compiler.set_constant("reflection_power", float(custom_config["reflection_power"]));
			else
				compiler.set_constant("reflection_power", 1.0f);

			if (custom_config.value_exists("min_reflection_angle"))
				compiler.set_constant("min_reflection_angle", math::deg2rad(float(custom_config["min_reflection_angle"])));
			else
				compiler.set_constant("min_reflection_angle", 0.0f);

			if (custom_config.value_exists("specular_color_multiplier"))
				compiler.set_constant("specular_color_multiplier", float4(custom_config["specular_color_multiplier"]));
			else
				compiler.set_constant("specular_color_multiplier", float4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (configuration.use_normal_texture)
			compiler.set_texture("t_normal", shared_string(pcstr(custom_config["texture_normal"])), is_static_mesh, debug_last_mips);

		if (configuration.use_sequence)
		{
			compiler.set_constant(
				"sequence_parameters",
				float4(
					float(custom_config["constant_sequence_array_width"]),
					float(custom_config["constant_sequence_array_height"]),
					float(custom_config["constant_sequence_start_frame_index"]),
					float(custom_config["constant_sequence_play_speed"])
				));
		}

		if (configuration.use_detail_normal_texture)
		{
			float4 detail_normal_parameters(1.0f, 1.0f, 1.0f, 1.0f);

			if (custom_config.value_exists("texture_detail_normal"))
				compiler.set_texture("t_detail_normal", shared_string(pcstr(custom_config["texture_detail_normal"])), is_static_mesh, debug_last_mips);

			if (custom_config.value_exists("scale_detail_normal"))
			{
				float value = float(custom_config["scale_detail_normal"]);
				detail_normal_parameters.x = value;
				detail_normal_parameters.y = value;
			}
			if (custom_config.value_exists("tile_detail_normal"))
			{
				float value = float(custom_config["tile_detail_normal"]);
				detail_normal_parameters.z = value;
				detail_normal_parameters.w = value;
			}

			compiler.set_constant("detail_normal_parameters", detail_normal_parameters);
		}

		if (configuration.use_detail_texture)
		{
			if (custom_config.value_exists("texture_ditail_diffuse"))
				compiler.set_texture("t_detail", shared_string(pcstr(custom_config["texture_ditail_diffuse"])), is_static_mesh, debug_last_mips);

			if (custom_config.value_exists("constant_ditail_texture_tile"))
				compiler.set_constant("ditail_texture_tile", float(custom_config["constant_ditail_texture_tile"]));
		}
		if (configuration.use_parallax)
		{
			if (custom_config.value_exists("constant_parallax_scale"))
				compiler.set_constant("constant_parallax_scale", float(custom_config["constant_parallax_scale"]));

			compiler.set_texture("t_height_map", shared_string(pcstr(custom_config["texture_bump"])), is_static_mesh, debug_last_mips);
		}
		float4 specular_intensity_ranges(0.0f, 1.0f, 0.0f, 0.0f);

		if (configuration.use_specular_intensity_texture)
		{
			compiler.set_texture("t_specular_intensity", shared_string(pcstr(custom_config["texture_specular_intensity"])), is_static_mesh, debug_last_mips);

			if (custom_config.value_exists("constant_specular_intensity_min"))
			{
				specular_intensity_ranges.x = float(custom_config["constant_specular_intensity_min"]);
				specular_intensity_ranges.y = float(custom_config["constant_specular_intensity_max"]) - specular_intensity_ranges.x;
			}
		}

		compiler.set_constant("specular_intensity_ranges", specular_intensity_ranges);

		if (custom_config.value_exists("constant_specular_color") && custom_config.value_exists("constant_specular_color_multiplier"))
			compiler.set_constant("specular_color_parameter", float4(custom_config["constant_specular_color"]).xyz() * float3(custom_config["constant_specular_color_multiplier"]));

		compiler.set_constant("solid_color_specular", solid_color_specular);

		float4 solid_material_params(0.0f, 0.0f, 0.0f, 0.0f);
		float4 specular_fresnel_roughness_parameters(0.0f, 0.0f, 0.0f, 0.0f);

		if (configuration.use_roughness_texture)
		{
			compiler.set_texture("t_roughness", shared_string(pcstr(custom_config["texture_roughness"])), is_static_mesh, debug_last_mips);
			if (custom_config.value_exists("constant_roughness_min"))
			{
				specular_fresnel_roughness_parameters.z = float(custom_config["constant_roughness_min"]);
				specular_fresnel_roughness_parameters.w = float(custom_config["constant_roughness_max"]) - specular_fresnel_roughness_parameters.z;
			}
		}
		else if (custom_config.value_exists("constant_roughness"))
			specular_fresnel_roughness_parameters.z = float(custom_config["constant_roughness"]);

		if (configuration.use_fresnel_texture)
		{
			compiler.set_texture("t_fresnel", shared_string(pcstr(custom_config["texture_fresnel"])), is_static_mesh, debug_last_mips);
			if (custom_config.value_exists("constant_fresnel_min"))
			{
				specular_fresnel_roughness_parameters.x = float(custom_config["constant_fresnel_min"]);
				specular_fresnel_roughness_parameters.y = float(custom_config["constant_fresnel_max"]) - specular_fresnel_roughness_parameters.x;
			}
		}
		else
		{
			if (custom_config.value_exists("constant_fresnel"))
				specular_fresnel_roughness_parameters.x = float(custom_config["constant_fresnel"]);
		}
		compiler.set_constant("specular_fresnel_roughness_parameters", specular_fresnel_roughness_parameters);
		if (configuration.use_translucency_texture)
		{
			compiler.set_texture("t_translucency", shared_string(pcstr(custom_config["texture_translucency"])), is_static_mesh, debug_last_mips);
			solid_material_params.z = float(custom_config["constant_translucency"]);
		}

		if (configuration.use_variation_mask)
		{
			compiler.set_constant(
				"packed_variation_mask_parameters",
				float4(
					custom_config.value_exists("constant_variation_position_devider") ? float(custom_config["constant_variation_position_devider"]) : 1.0f,
					custom_config.value_exists("constant_variation_rotate") ? float(custom_config["constant_variation_rotate"]) : 1.0f,
					custom_config.value_exists("constant_variation_scale") ? float(custom_config["constant_variation_scale"]) : 1.0f,
					custom_config.value_exists("constant_variation_multiply") ? float(custom_config["constant_variation_multiply"]) : 1.0f
				)
			);

			if (custom_config.value_exists("constant_variation_color"))
				compiler.set_constant("variation_color", float4(custom_config["constant_variation_color"]));

			if (custom_config.value_exists("texture_variation_mask"))
				compiler.set_texture("t_variation_mask", shared_string(pcstr(custom_config["texture_variation_mask"])), is_static_mesh, debug_last_mips);
			else
				compiler.set_texture("t_variation_mask", "", 0, false, u32(-1));
		}
		if (configuration.use_uv_scrolling)
		{
			float4 uv_scrolling_parameters;
			uv_scrolling_parameters.x = custom_config.value_exists("normal_map_scroll_u_direction") ? float(custom_config["normal_map_scroll_u_direction"]) : 0.0f;
			uv_scrolling_parameters.y = custom_config.value_exists("normal_map_scroll_v_direction") ? float(custom_config["normal_map_scroll_v_direction"]) : 0.0f;
			uv_scrolling_parameters.z = custom_config.value_exists("all_maps_scroll_u_direction") ? float(custom_config["all_maps_scroll_u_direction"]) : 0.0f;
			uv_scrolling_parameters.w = custom_config.value_exists("all_maps_scroll_v_direction") ? float(custom_config["all_maps_scroll_v_direction"]) : 0.0f;

			compiler.set_constant("uv_scrolling_parameters", uv_scrolling_parameters);
		}

		compiler.set_constant("solid_material_params", solid_material_params);

		compile_end(compiler);
	}
	// fill rsm backend
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base_lpv", "fill_reflective_shadow_map_backed", compiler, &local_configuration, custom_config);
		compile_end(compiler);
	}

	// fill vertex color
	{
		shader_configuration local_configuration;
		local_configuration.use_diffuse_texture = bool(custom_config["use_tdiffuse"]);

		compile_begin("fill_vertex_color", "fill_vertex_color", compiler, &local_configuration, custom_config);

		compiler.set_depth(false, false);
		compiler.set_stencil(false);

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", shared_string(pcstr(custom_config["texture_diffuse"])), is_static_mesh, debug_last_mips);

		if (custom_config.value_exists("constant_diffuse"))
			compiler.set_constant("diffuse_color_parameter", float4(custom_config["constant_diffuse"]).xyz());
		else
			compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));

		compile_end(compiler);
	}

	// fill rsm
	{
		shader_configuration local_configuration;
		local_configuration.use_diffuse_texture = bool(custom_config["use_tdiffuse"]);

		compile_begin("vertex_base", "fill_reflective_shadow_map", compiler, &local_configuration, custom_config);

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", shared_string(pcstr(custom_config["texture_diffuse"])), is_static_mesh, debug_last_mips);

		if (custom_config.value_exists("constant_diffuse"))
			compiler.set_constant("diffuse_color_parameter", float4(custom_config["constant_diffuse"]).xyz());
		else
			compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));

		compile_end(compiler);
	}

	// emissive
	{
		shader_configuration local_configuration;

		if (bool(custom_config["use_emissive"]))
			local_configuration.use_emissive = bool(custom_config["use_emissive_map"]) ? 2 : 1;

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
			compiler.set_texture("t_emission", shared_string(pcstr(custom_config["texture_emissive"])), is_static_mesh, debug_last_mips);

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
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, is_static_mesh, debug_last_mips);

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
		local_configuration.use_thickness_map = custom_config.value_exists("use_thickness_map") ? bool(custom_config["use_thickness_map"]) : false;
		local_configuration.use_subsurface_scattering_mask_map = custom_config.value_exists("use_subsurface_scattering_mask_map") ? bool(custom_config["use_subsurface_scattering_mask_map"]) : false;

		compile_begin("vertex_base", "subsurface_scattering", compiler, &local_configuration, custom_config);
		compiler.set_depth(true, false);

		float alpha_ref = 0.25f;

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, is_static_mesh, debug_last_mips);

		if (local_configuration.use_alpha_test && custom_config.value_exists("alpha_ref"))
			alpha_ref = float(custom_config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		compiler.set_texture("t_diffuse_lighting", "$user$accum_diffuse", 0, false, u32(-1));
		compiler.set_texture("t_specular_lighting", "$user$accum_specular", 0, false, u32(-1));
		compiler.set_texture("t_diffuse", "$user$albedo", 0, false, u32(-1));
		compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
		compiler.set_texture("t_normal", "$user$normal", 0, false, u32(-1));
		compiler.set_texture("t_sun_translucensy_help_data", "$user$sun_translucensy_help_data", 0, false, u32(-1));

		if (local_configuration.use_thickness_map)
			compiler.set_texture("t_thickness", pcstr(custom_config["texture_thickness"]), 0, false, u32(-1));

		if (local_configuration.use_subsurface_scattering_mask_map)
			compiler.set_texture("t_sss_mask", pcstr(custom_config["texture_sss_mask"]), 0, false, u32(-1));

		compiler.set_cull_mode(D3D_CULL_NONE);
		compile_end(compiler);
	}

	// z only
	{
		shader_configuration local_configuration;

		local_configuration.use_alpha_test = custom_config.value_exists("use_alpha_test") ? bool(custom_config["use_alpha_test"]) : false;
		local_configuration.use_diffuse_texture = custom_config.value_exists("use_tdiffuse") ? bool(custom_config["use_tdiffuse"]) : false;

		compile_begin("vertex_base", "z_only", compiler, &local_configuration, custom_config);
		compiler.set_depth(true, true);

		float alpha_ref = 0.25f;

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, is_static_mesh, debug_last_mips);

		if (local_configuration.use_alpha_test && custom_config.value_exists("alpha_ref"))
			alpha_ref = float(custom_config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);
		compiler.color_write_enable(D3D_COLOR_WRITE_ENABLE_NONE);
		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
