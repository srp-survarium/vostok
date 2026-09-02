// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_POST_PROCESS_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_POST_PROCESS_PARAMETERS_H_INCLUDED
#include <vostok/intrusive_ptr.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/render/core/resource_intrusive_base.h>

#include "res_texture.h"

namespace vostok {
namespace render {

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

struct post_process_parameters {
	post_process_parameters( );

	float3			dof_height_lights;
	float			dof_focus_power;
	float			dof_focus_region;
	float			dof_focus_distance;
	float			dof_far_blur_amount;
	float			dof_near_blur_amount;
	bool			enable_dof;
	bool			use_bokeh_dof;
	float			bokeh_dof_radius;
	float			bokeh_dof_density;
	bool			use_bokeh_image;
	bool			use_image_grain;
	float			image_grain_scale;
	u32				image_grain_update_frequency;
	float			bloom_scale;
	float			bloom_max_color;
	u32				blur_kernel;
	float			frame_dark_desaturation_limit;
	float3			bloom_halo_color;
	bool			enable_bloom;
	bool			enable_ssao;
	float			ssao_saturation;
	float			ssao_radius_scale;
	float			frame_desaturation;
	float3			frame_height_lights;
	float3			frame_mid_tones;
	float3			frame_shadows;
	float3			frame_fade_color;
	float			frame_fade_amount;
	bool			enable_scene;
	bool			enable_advanced_bloom;
	float3			environment_sun_position;
	float3			environment_far_fog_color;
	float3			environment_ambient_color;
	float3			environment_sun_color;
	float			environment_far_fog_distance;
	float			environment_near_fog_distance;
	float			environment_shadow_transparency;
	float			environment_fog_alpha;
	float			aberration_max_variance;
	float			aberration_min_variance;
	float			aberration_power;
	float4			environment_skylight_lower_color;
	float4			environment_skylight_upper_color;
	float4			god_rays_color_0;
	float4			god_rays_color_1;
	float			god_rays_color_blend_power;
	float			god_rays_intensity;
	float			god_rays_attenuation_power;
	float			environment_rain_angle_x;
	float			environment_rain_angle_y;
	float			environment_rain_density;
	float			environment_rain_speed;
	bool			environment_use_rain;
	float4			environment_skylight_parameters;
	float			adaptation_speed;
	float			tonemap_bright_threshold;
	float			tonemap_median;
	float			tonemap_darkness_threshold;
	float			tonemap_middle_gray;
	float3			wind_direction;
	float			wind_strength;
	bool			use_color_grading_lut;
	float4			environment_skycolor[6];
	float4			environment_suncolor;
	bool			use_environment_skycolor;
	res_texture_ptr	color_grading_texture;
	res_texture_ptr	lens_flares_mask_texture;
	res_texture_ptr	sky_clouds_texture;
	res_texture_ptr	sun_moon_texture;
	float3			sun_moon_color;
	float			sun_moon_billboard_scale;
	bool			use_sun_moon_atmosphere_illumination;
	float			sky_clouds_u_tile;
	float4			sky_clouds_color;
	float			sky_clouds_fog_power;
	float			sky_clouds_fog_up_limit;
	float			sky_clouds_rotation;
	u32				sky_clouds_blend_mode;
	float			filmic_tonemap_shoulder_strength;
	float			filmic_tonemap_linear_strength;
	float			filmic_tonemap_linear_angle;
	float			filmic_tonemap_toe_strength;
	float			filmic_tonemap_toe_numerator;
	float			filmic_tonemap_toe_denominator;
	float			filmic_tonemap_liner_white;
	float			vignette_power;
	bool			use_image_space_reflections;
	float3			skylight_color;
	float			skylight_lower;
	float			skylight_upper;
	float			skylight_power;
	float			skylight_multiplier;
	bool			use_dynamic_lens_flares;
	float			environment_probes_diffuse_instensity_multiplier;
	float			environment_probes_specular_instensity_multiplier;
	float			environment_rain_wet_intensity;
	float			atmosphere_kresun_multiplier;
	float			atmosphere_kmesun_multiplier;
	float			atmosphere_kr4pi_multiplier;
	float			atmosphere_km4pi_multiplier;
	bool			atmosphere_params_changed;
	u32				dof_blur_kernel;
	u32				environment_rain_num_cones;
	float			environment_rain_u_scale;
	float			environment_rain_v_scale;
	float			environment_rain_random_rotaion_speed;
	float			environment_rain_random_rotaion_radius;
	float			environment_rain_random_base_offset;
	float			environment_rain_radius_scale;
	u32				environment_rain_start_cone_index;
	float			lens_flares_multiplier;
	float			atmosphere_inscatter_distance;
	float			atmosphere_inscatter_power;
	bool			use_atmosphere_inscattering_on_geometry;
	bool			use_aberration;
};

STATIC_SIZE_ASSERT( post_process_parameters, 0x2D4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_POST_PROCESS_PARAMETERS_H_INCLUDED
