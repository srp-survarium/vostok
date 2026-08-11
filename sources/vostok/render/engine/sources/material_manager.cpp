////////////////////////////////////////////////////////////////////////////
//	Created		: 17.05.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "material_manager.h"
#include "effect_material_base.h"
#include <vostok/fs_watcher.h>
#include "render_model.h"
#include "render_particle_emitter_instance.h"
#include <vostok/particle/world.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/destroy_data_helper.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include "post_process_parameters.h"
#include <vostok/render/core/resource_manager.h>
#include "material_effects.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/untyped_buffer.h>

#include <vostok/render/facade/material_effects_instance_cook_data.h>

namespace vostok {
namespace render {


class material_shader_base: public effect_material_base
{
	material_shader_base(enum_vertex_input_type /*vertex_input_type*/, material_ptr /*mtl*/)
	{

	}
	u32				m_vertex_type;
	material_ptr	m_mtl;
	res_effect_ptr	m_shader;
};

material_manager::material_manager()
{
#ifndef MASTER_GOLD
	m_watcher_materials_subscribe_id	=	vostok::resources::subscribe_watcher(get_materials_path(),
										boost::bind(&material_manager::on_material_source_changed, this, _1));

	m_watcher_material_instanced_subscribe_id	=
										vostok::resources::subscribe_watcher(get_material_instances_path(),
										boost::bind(&material_manager::on_material_source_changed, this, _1));
#endif // #ifndef MASTER_GOLD
}

material_manager::~material_manager()
{
#ifndef MASTER_GOLD
	vostok::resources::unsubscribe_watcher	(m_watcher_materials_subscribe_id);
	vostok::resources::unsubscribe_watcher	(m_watcher_material_instanced_subscribe_id);
#endif // #ifndef MASTER_GOLD

	{
		material_effects_entries_type	temp_material_effects;
		m_material_effects.swap			(temp_material_effects);
		m_material_effects.clear		();
	}
//	resources::unregister_cook(resources::material_effects_instance_class);
}

static pcstr resources_materials_string						=	"resources/materials";
static pcstr resources_material_instances_string			=	"resources/material_instances";
static pcstr resources_materials_string_with_slash			=	"resources/materials/";
// 210 target lines are likely retail-compiled-out source.
void material_manager::initialize_post_process_parameters(post_process_parameters* out_post_process_parameters_ptr, material_ptr mtl, bool force_recompilation)
{
	VOSTOK_UNREFERENCED_PARAMETER									(force_recompilation);
	
	//remove_post_process_parameters								(out_post_process_parameters_ptr);
	u32 crc														= 0;
	custom_config_ptr material_config_ptr						= create_custom_config(mtl->m_config->get_root()["material"], crc, false);
	custom_config_value const& material_config					= material_config_ptr->root();
	
	if (!material_config.value_exists("post_process"))
		return;
	
	fs_new::virtual_path_string mtl_name						= mtl->get_material_name();
	
	m_post_process_parameters[out_post_process_parameters_ptr]	= mtl_name;
	
	custom_config_value const& config							= material_config["post_process"];
	
	post_process_parameters& out_post_process_parameters		= *out_post_process_parameters_ptr;
	out_post_process_parameters									=  post_process_parameters();
	if (config.value_exists("atmosphere_kresun_multiplier"))
	{
		out_post_process_parameters.atmosphere_params_changed		= true;
		out_post_process_parameters.atmosphere_kresun_multiplier	= float(config["atmosphere_kresun_multiplier"]["value"]);
	}

	if (config.value_exists("atmosphere_kmesun_multiplier"))
	{
		out_post_process_parameters.atmosphere_params_changed		= true;
		out_post_process_parameters.atmosphere_kmesun_multiplier	= float(config["atmosphere_kmesun_multiplier"]["value"]);
	}

	if (config.value_exists("atmosphere_kr4pi_multiplier"))
	{
		out_post_process_parameters.atmosphere_params_changed		= true;
		out_post_process_parameters.atmosphere_kr4pi_multiplier	= float(config["atmosphere_kr4pi_multiplier"]["value"]);
	}

	if (config.value_exists("atmosphere_km4pi_multiplier"))
	{
		out_post_process_parameters.atmosphere_params_changed		= true;
		out_post_process_parameters.atmosphere_km4pi_multiplier	= float(config["atmosphere_km4pi_multiplier"]["value"]);
	}

	if (config.value_exists("vignette_power"))
		out_post_process_parameters.vignette_power				= float(config["vignette_power"]["value"]);

	if (config.value_exists("use_image_grain"))
		out_post_process_parameters.use_image_grain				= bool(config["use_image_grain"]["value"]);

	if (config.value_exists("use_dynamic_lens_flares"))
		out_post_process_parameters.use_dynamic_lens_flares		= bool(config["use_dynamic_lens_flares"]["value"]);

	if (config.value_exists("image_grain_scale"))
		out_post_process_parameters.image_grain_scale			= float(config["image_grain_scale"]["value"]);

	if (config.value_exists("image_grain_update_frequency"))
		out_post_process_parameters.image_grain_update_frequency	= u32(config["image_grain_update_frequency"]["value"]);

	if (config.value_exists("use_image_space_reflections"))
		out_post_process_parameters.use_image_space_reflections	= bool(config["use_image_space_reflections"]["value"]);

	if (config.value_exists("dof_height_lights"))
		out_post_process_parameters.dof_height_lights			= float3(config["dof_height_lights"]["value"]);
	// 4 target lines are likely retail-compiled-out source.
	if (config.value_exists("dof_focus_region"))
		out_post_process_parameters.dof_focus_region			= float(config["dof_focus_region"]["value"]);

	if (config.value_exists("dof_focus_distance"))
		out_post_process_parameters.dof_focus_distance			= float(config["dof_focus_distance"]["value"]);

	if (config.value_exists("dof_near_blurriness_amount"))
		out_post_process_parameters.dof_near_blur_amount		= float(config["dof_near_blurriness_amount"]["value"]);

	if (config.value_exists("dof_near_blurriness_far"))
		out_post_process_parameters.dof_far_blur_amount			= float(config["dof_near_blurriness_far"]["value"]);
	
	if (config.value_exists("use_bokeh_dof"))
		out_post_process_parameters.use_bokeh_dof				= bool(config["use_bokeh_dof"]["value"]);
	
	if (config.value_exists("bokeh_dof_radius"))
		out_post_process_parameters.bokeh_dof_radius			= float(config["bokeh_dof_radius"]["value"]);

	if (config.value_exists("bokeh_dof_density"))
		out_post_process_parameters.bokeh_dof_density			= float(config["bokeh_dof_density"]["value"]);

	if (config.value_exists("use_bokeh_image"))
		out_post_process_parameters.use_bokeh_image				= bool(config["use_bokeh_image"]["value"]);
	
	if (config.value_exists("enable_ssao"))
		out_post_process_parameters.enable_ssao					= bool(config["enable_ssao"]["value"]);
	
	if (config.value_exists("ssao_saturation"))
		out_post_process_parameters.ssao_saturation				= float(config["ssao_saturation"]["value"]);
	
	if (config.value_exists("ssao_radius_scale"))
		out_post_process_parameters.ssao_radius_scale			= float(config["ssao_radius_scale"]["value"]);
	
	if (config.value_exists("god_rays_color_0"))
		out_post_process_parameters.god_rays_color_0				= float4(config["god_rays_color_0"]["value"]);

	if (config.value_exists("god_rays_color_1"))
		out_post_process_parameters.god_rays_color_1				= float4(config["god_rays_color_1"]["value"]);

	if (config.value_exists("god_rays_color_blend_power"))
		out_post_process_parameters.god_rays_color_blend_power	= float(config["god_rays_color_blend_power"]["value"]);

	if (config.value_exists("god_rays_intensity"))
		out_post_process_parameters.god_rays_intensity			= float(config["god_rays_intensity"]["value"]);

	if (config.value_exists("god_rays_attenuation_power"))
		out_post_process_parameters.god_rays_attenuation_power	= float(config["god_rays_attenuation_power"]["value"]);

	if (config.value_exists("environment_rain_wet_intensity"))
		out_post_process_parameters.environment_rain_wet_intensity = float(config["environment_rain_wet_intensity"]["value"]);

	if (config.value_exists("environment_probes_diffuse_instensity_multiplier"))
		out_post_process_parameters.environment_probes_diffuse_instensity_multiplier = float(config["environment_probes_diffuse_instensity_multiplier"]["value"]);

	if (config.value_exists("environment_probes_specular_instensity_multiplier"))
		out_post_process_parameters.environment_probes_specular_instensity_multiplier = float(config["environment_probes_specular_instensity_multiplier"]["value"]);

	if (config.value_exists("atmosphere_inscatter_distance"))
		out_post_process_parameters.atmosphere_inscatter_distance = float(config["atmosphere_inscatter_distance"]["value"]);

	if (config.value_exists("atmosphere_inscatter_power"))
		out_post_process_parameters.atmosphere_inscatter_power	= float(config["atmosphere_inscatter_power"]["value"]);

	if (config.value_exists("use_atmosphere_inscattering_on_geometry"))
		out_post_process_parameters.use_atmosphere_inscattering_on_geometry = bool(config["use_atmosphere_inscattering_on_geometry"]["value"]);

	if (config.value_exists("dof_blur_kernel"))
		out_post_process_parameters.dof_blur_kernel				= u32(config["dof_blur_kernel"]["value"]);

	if (config.value_exists("enable_advanced_bloom"))
		out_post_process_parameters.enable_advanced_bloom		= bool(config["enable_advanced_bloom"]["value"]);

	if (config.value_exists("bloom_scale"))
		out_post_process_parameters.bloom_scale					= float(config["bloom_scale"]["value"]);

	if (config.value_exists("blur_kernel"))
		out_post_process_parameters.blur_kernel					= u32(config["blur_kernel"]["value"]);

	if (config.value_exists("bloom_halo_color"))
		out_post_process_parameters.bloom_halo_color				= float3(config["bloom_halo_color"]["value"]);

	if (config.value_exists("bloom_max_color"))
		out_post_process_parameters.bloom_max_color				= float(config["bloom_max_color"]["value"]);

	if (config.value_exists("frame_desaturation"))
		out_post_process_parameters.frame_desaturation			= float(config["frame_desaturation"]["value"]);

	if (config.value_exists("frame_dark_desaturation_limit"))
		out_post_process_parameters.frame_dark_desaturation_limit = float(config["frame_dark_desaturation_limit"]["value"]);


	if (config.value_exists("environment_rain_num_cones"))
		out_post_process_parameters.environment_rain_num_cones	= u32(float(config["environment_rain_num_cones"]["value"]));

	if (config.value_exists("environment_rain_u_scale"))
		out_post_process_parameters.environment_rain_u_scale		= float(config["environment_rain_u_scale"]["value"]);

	if (config.value_exists("environment_rain_v_scale"))
		out_post_process_parameters.environment_rain_v_scale		= float(config["environment_rain_v_scale"]["value"]);

	if (config.value_exists("environment_rain_random_rotaion_speed"))
		out_post_process_parameters.environment_rain_random_rotaion_speed = float(config["environment_rain_random_rotaion_speed"]["value"]);

	if (config.value_exists("environment_rain_random_rotaion_radius"))
		out_post_process_parameters.environment_rain_random_rotaion_radius = float(config["environment_rain_random_rotaion_radius"]["value"]);

	if (config.value_exists("environment_rain_random_base_offset"))
		out_post_process_parameters.environment_rain_random_base_offset = float(config["environment_rain_random_base_offset"]["value"]);

	if (config.value_exists("environment_rain_radius_scale"))
		out_post_process_parameters.environment_rain_radius_scale	= float(config["environment_rain_radius_scale"]["value"]);

	if (config.value_exists("environment_rain_start_cone_index"))
		out_post_process_parameters.environment_rain_start_cone_index = u32(config["environment_rain_start_cone_index"]["value"]);


	if (config.value_exists("frame_height_lights"))
		out_post_process_parameters.frame_height_lights			= float3(config["frame_height_lights"]["value"]);
	
	if (config.value_exists("frame_mid_tones"))
		out_post_process_parameters.frame_mid_tones				= float3(config["frame_mid_tones"]["value"]);

	if (config.value_exists("filmic_tonemap_shoulder_strength"))
		out_post_process_parameters.filmic_tonemap_shoulder_strength = float(config["filmic_tonemap_shoulder_strength"]["value"]);

	if (config.value_exists("filmic_tonemap_linear_strength"))
		out_post_process_parameters.filmic_tonemap_linear_strength = float(config["filmic_tonemap_linear_strength"]["value"]);

	if (config.value_exists("filmic_tonemap_linear_angle"))
		out_post_process_parameters.filmic_tonemap_linear_angle	= float(config["filmic_tonemap_linear_angle"]["value"]);

	if (config.value_exists("filmic_tonemap_toe_strength"))
		out_post_process_parameters.filmic_tonemap_toe_strength	= float(config["filmic_tonemap_toe_strength"]["value"]);

	if (config.value_exists("filmic_tonemap_toe_numerator"))
		out_post_process_parameters.filmic_tonemap_toe_numerator	= float(config["filmic_tonemap_toe_numerator"]["value"]);

	if (config.value_exists("filmic_tonemap_toe_denominator"))
		out_post_process_parameters.filmic_tonemap_toe_denominator = float(config["filmic_tonemap_toe_denominator"]["value"]);

	if (config.value_exists("filmic_tonemap_liner_white"))
		out_post_process_parameters.filmic_tonemap_liner_white	= float(config["filmic_tonemap_liner_white"]["value"]);

	if (config.value_exists("skylight_color")) out_post_process_parameters.skylight_color = float4(config["skylight_color"]["value"]).xyz();
	if (config.value_exists("skylight_lower")) out_post_process_parameters.skylight_lower = float(config["skylight_lower"]["value"]);
	if (config.value_exists("skylight_upper")) out_post_process_parameters.skylight_upper = float(config["skylight_upper"]["value"]);
	if (config.value_exists("skylight_power")) out_post_process_parameters.skylight_power = float(config["skylight_power"]["value"]);
	if (config.value_exists("skylight_multiplier")) out_post_process_parameters.skylight_multiplier = float(config["skylight_multiplier"]["value"]);


	if (config.value_exists("frame_shadows"))
		out_post_process_parameters.frame_shadows				= float3(config["frame_shadows"]["value"]);
	// 5 target lines are likely retail-compiled-out source.
	if (config.value_exists("environment_far_fog_color") &&
		config.value_exists("environment_far_fog_distance") &&
		config.value_exists("environment_near_fog_distance"))
	{
		out_post_process_parameters.environment_far_fog_color		= float4(config["environment_far_fog_color"]["value"]).xyz();
		out_post_process_parameters.environment_far_fog_distance	= float(config["environment_far_fog_distance"]["value"]);
		out_post_process_parameters.environment_near_fog_distance	= float(config["environment_near_fog_distance"]["value"]);
	}
	// 4 target lines are likely retail-compiled-out source.
	if (config.value_exists("lens_flares_multiplier"))
		out_post_process_parameters.lens_flares_multiplier		= float(config["lens_flares_multiplier"]["value"]);

	if (config.value_exists("use_aberration"))
		out_post_process_parameters.use_aberration				= bool(config["use_aberration"]["value"]);


	if (config.value_exists("aberration_power"))
		out_post_process_parameters.aberration_power				= float(config["aberration_power"]["value"]);

	if (config.value_exists("aberration_max_variance"))
		out_post_process_parameters.aberration_max_variance		= float(config["aberration_max_variance"]["value"]);

	if (config.value_exists("aberration_min_variance"))
		out_post_process_parameters.aberration_min_variance		= float(config["aberration_min_variance"]["value"]);

	if (config.value_exists("environment_fog_alpha"))
		out_post_process_parameters.environment_fog_alpha		= float(config["environment_fog_alpha"]["value"]);
	

	if (config.value_exists("environment_ambient_color"))
		out_post_process_parameters.environment_ambient_color		= float4(config["environment_ambient_color"]["value"]).xyz();


	if (config.value_exists("environment_use_rain"))
		out_post_process_parameters.environment_use_rain			= bool(config["environment_use_rain"]["value"]);

	if (config.value_exists("environment_rain_angle_x"))
		out_post_process_parameters.environment_rain_angle_x		= float(config["environment_rain_angle_x"]["value"]);

	if (config.value_exists("environment_rain_angle_y"))
		out_post_process_parameters.environment_rain_angle_y		= float(config["environment_rain_angle_y"]["value"]);

	if (config.value_exists("environment_rain_density"))
		out_post_process_parameters.environment_rain_density		= float(config["environment_rain_density"]["value"]);

	if (config.value_exists("environment_rain_speed"))
		out_post_process_parameters.environment_rain_speed		= float(config["environment_rain_speed"]["value"]);
	
	if (config.value_exists("environment_sun_color"))
		out_post_process_parameters.environment_sun_color			= float4(config["environment_sun_color"]["value"]).xyz();
	
	if (config.value_exists("environment_shadow_transparency"))
		out_post_process_parameters.environment_shadow_transparency	= float(config["environment_shadow_transparency"]["value"]);
	
	if (config.value_exists("environment_skylight_upper_color"))
		out_post_process_parameters.environment_skylight_upper_color	= float4(config["environment_skylight_upper_color"]["value"]);	
	
	if (config.value_exists("environment_skylight_lower_color"))
		out_post_process_parameters.environment_skylight_lower_color	= float4(config["environment_skylight_lower_color"]["value"]);	


	if (
		config.value_exists("environment_skylight_upper_power")	&&
		config.value_exists("environment_skylight_lower_power")
		)
		// 4 target lines are likely retail-compiled-out source.
		out_post_process_parameters.environment_skylight_parameters		= float4(float(config["environment_skylight_upper_power"]["value"]), float(config["environment_skylight_lower_power"]["value"]), 0.0f, 0.0f);
	
	if (config.value_exists("adaptation_speed"))
		out_post_process_parameters.adaptation_speed			= float(config["adaptation_speed"]["value"]);
	
	if (config.value_exists("tonemap_bright_threshold"))
		out_post_process_parameters.tonemap_bright_threshold	= float(config["tonemap_bright_threshold"]["value"]);
	if (config.value_exists("tonemap_median"))
		out_post_process_parameters.tonemap_median				= float(config["tonemap_median"]["value"]);
	if (config.value_exists("tonemap_darkness_threshold"))
		out_post_process_parameters.tonemap_darkness_threshold	= float(config["tonemap_darkness_threshold"]["value"]);
	if (config.value_exists("tonemap_middle_gray"))
		out_post_process_parameters.tonemap_middle_gray			= float(config["tonemap_middle_gray"]["value"]);
	// 3 target lines are likely retail-compiled-out source.
	if (config.value_exists("wind_direction")) 
		out_post_process_parameters.wind_direction				= float3(config["wind_direction"]["value"]);
	
	if (config.value_exists("wind_strength")) 
		out_post_process_parameters.wind_strength				= float(config["wind_strength"]["value"]);

	if (config.value_exists("enable_bloom"))
		out_post_process_parameters.enable_bloom				= bool(config["enable_bloom"]["value"]);

#if VOSTOK_PLATFORM_WINDOWS
	// 4 target lines are likely retail-compiled-out source.
	if (config.value_exists("environment_suncolor"))
		out_post_process_parameters.environment_suncolor			= float4(config["environment_suncolor"]["value"]);

	if (config.value_exists("environment_skycolor_up"))
		out_post_process_parameters.environment_skycolor[3]		= float4(config["environment_skycolor_up"]["value"]);

	if (config.value_exists("environment_skycolor_down"))
		out_post_process_parameters.environment_skycolor[2]		= float4(config["environment_skycolor_down"]["value"]);
	
	if (config.value_exists("environment_skycolor_x"))
		out_post_process_parameters.environment_skycolor[1]		= float4(config["environment_skycolor_x"]["value"]);
	
	if (config.value_exists("environment_skycolor_minus_x"))
		out_post_process_parameters.environment_skycolor[0]		= float4(config["environment_skycolor_minus_x"]["value"]);
	
	if (config.value_exists("environment_skycolor_z"))
		out_post_process_parameters.environment_skycolor[5]		= float4(config["environment_skycolor_z"]["value"]);
	
	if (config.value_exists("environment_skycolor_minus_z"))
		out_post_process_parameters.environment_skycolor[4]		= float4(config["environment_skycolor_minus_z"]["value"]);
	
	if (config.value_exists("environment_use_skycolor"))
		out_post_process_parameters.use_environment_skycolor	= bool(config["environment_use_skycolor"]["value"]);
	
#pragma message( VOSTOK_TODO("Add color_grading_texture to post_process_parameters in other platforms") )
	
	if (config.value_exists("use_color_grading_lut"))
		out_post_process_parameters.use_color_grading_lut		= bool(config["use_color_grading_lut"]["value"]);
	

	if (config.value_exists("color_grading_texture"))
		out_post_process_parameters.color_grading_texture		= resource_manager::ref().create_texture(pcstr(config["color_grading_texture"]["value"]), 0, 0, false, true, true, u32(-1));

	if (config.value_exists("lens_flares_mask_texture") &&
		!strings::equal(pcstr(config["lens_flares_mask_texture"]["value"]), ""))
		out_post_process_parameters.lens_flares_mask_texture		= resource_manager::ref().create_texture(pcstr(config["lens_flares_mask_texture"]["value"]), 0, 0, false, true, true, u32(-1));
	else
		out_post_process_parameters.lens_flares_mask_texture		= resource_manager::ref().create_texture("engine/lensdirt_highc", 0, 0, false, true, true, u32(-1));
	if (config.value_exists("use_sky_clouds") && bool(config["use_sky_clouds"]["value"]))
	{
		if (config.value_exists("sky_clouds_texture") && !strings::equal(pcstr(config["sky_clouds_texture"]["value"]), ""))
			out_post_process_parameters.sky_clouds_texture			= resource_manager::ref().create_texture(pcstr(config["sky_clouds_texture"]["value"]), 0, 0, false, true, true, u32(-1));

		if (config.value_exists("sky_clouds_color") && config.value_exists("sky_clouds_color_multiplier"))
			out_post_process_parameters.sky_clouds_color				= math::pow(float4(config["sky_clouds_color"]["value"]), 2.2f) * float(config["sky_clouds_color_multiplier"]["value"]);

		if (config.value_exists("sky_clouds_fog_power") && config.value_exists("sky_clouds_fog_up_limit"))
		{
			out_post_process_parameters.sky_clouds_fog_power		= float(config["sky_clouds_fog_power"]["value"]);
			out_post_process_parameters.sky_clouds_fog_up_limit		= float(config["sky_clouds_fog_up_limit"]["value"]);
		}

		if (config.value_exists("sky_clouds_u_tile"))
			out_post_process_parameters.sky_clouds_u_tile			= float(config["sky_clouds_u_tile"]["value"]);

		if (config.value_exists("sky_clouds_rotation"))
			out_post_process_parameters.sky_clouds_rotation			= float(config["sky_clouds_rotation"]["value"]);

		if (config.value_exists("sky_clouds_blend_mode"))
			out_post_process_parameters.sky_clouds_blend_mode		= u32(config["sky_clouds_blend_mode"]["value"]);
	}
	else
		out_post_process_parameters.sky_clouds_texture				= 0;

	if (config.value_exists("use_sun_moon_texture") && bool(config["use_sun_moon_texture"]["value"]))
	{
		if (config.value_exists("sun_moon_texture") && !strings::equal(pcstr(config["sun_moon_texture"]["value"]), ""))
			out_post_process_parameters.sun_moon_texture			= resource_manager::ref().create_texture(pcstr(config["sun_moon_texture"]["value"]), 0, 0, false, true, true, u32(-1));

		if (config.value_exists("sun_moon_color_multiplier") && config.value_exists("sun_moon_color"))
			out_post_process_parameters.sun_moon_color				= math::pow(float4(config["sun_moon_color"]["value"]), 2.2f).xyz() * float(config["sun_moon_color_multiplier"]["value"]);

		if (config.value_exists("sun_moon_billboard_scale"))
			out_post_process_parameters.sun_moon_billboard_scale	= float(config["sun_moon_billboard_scale"]["value"]);

		if (config.value_exists("use_sun_moon_atmosphere_illumination"))
			out_post_process_parameters.use_sun_moon_atmosphere_illumination = bool(config["use_sun_moon_atmosphere_illumination"]["value"]);
	}
	else
		out_post_process_parameters.sun_moon_texture				= 0;


#endif // #if VOSTOK_PLATFORM_WINDOWS
}


void material_manager::remove_material_effects(material_effects_instance_ptr const& in_material_effects_instance)
{
	material_effects_entries_type::iterator it_begin	= m_material_effects.begin();
	material_effects_entries_type::iterator it_end		= m_material_effects.end();
	material_effects_entries_type::iterator it			= it_begin;
	for ( ; it != it_end; ++it)
	{
		if (*it->m_material_effects_instance_ptr == in_material_effects_instance)
			m_material_effects.erase(it);
	}
}
// 3 target lines are likely retail-compiled-out source.
void material_manager::add_material_effects(material_effects_instance_ptr& in_material_effects_instance, fs_new::virtual_path_string const& in_material_name)
{
	material_effects_entries_type::iterator it_begin	= m_material_effects.begin();
	material_effects_entries_type::iterator it_end		= m_material_effects.end();
	material_effects_entries_type::iterator it			= it_begin;
	for ( ; it != it_end; ++it)
	{
		if (*it->m_material_effects_instance_ptr == in_material_effects_instance)
			return;
	}
	
	m_material_effects.push_back(
		material_effects_entry(
			&in_material_effects_instance, 
			in_material_name
		)
	);
}

} // namespace render
} // namespace vostok
