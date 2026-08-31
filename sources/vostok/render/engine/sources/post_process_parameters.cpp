#include "pch.h"
#include "post_process_parameters.h"

#include <vostok/render/core/resource_manager.h>

namespace survarium {

extern float s_camera_far_plane;

} // namespace survarium

namespace vostok {
namespace render {

post_process_parameters::post_process_parameters( )
{
	dof_height_lights				= vostok::math::float3( 1.0f, 1.0f, 1.0f );
	dof_focus_power					= 0.6f;
	dof_focus_region				= 20.0f;
	dof_focus_distance				= 30.0f;
	dof_far_blur_amount				= 0.0f;
	dof_near_blur_amount			= 0.0f;
	enable_dof						= true;
	use_bokeh_dof					= false;
	bokeh_dof_radius				= 8.0f;
	bokeh_dof_density				= 1.0f;
	use_bokeh_image					= false;
	use_image_grain					= true;
	image_grain_scale				= 0.02f;
	image_grain_update_frequency	= 1;
	bloom_scale						= 0.5f;
	bloom_max_color					= 2.0f;
	blur_kernel						= 3;
	frame_dark_desaturation_limit	= 0.04f;
	bloom_halo_color				= vostok::math::float3( 1.0f, 1.0f, 1.0f );
	enable_bloom					= false;
	enable_ssao						= true;
	ssao_saturation					= 1.0f;
	ssao_radius_scale				= 1.0f;
	frame_desaturation				= 0.0f;
	frame_height_lights				= vostok::math::float3( 1.0f, 1.0f, 1.0f );
	frame_mid_tones					= vostok::math::float3( 1.0f, 1.0f, 1.0f );
	frame_shadows					= vostok::math::float3( 0.0f, 0.0f, 0.0f );
	frame_fade_color				= vostok::math::float3( 0.0f, 0.0f, 0.0f );
	frame_fade_amount				= 0.0f;
	enable_scene					= true;
	enable_advanced_bloom			= true;
	environment_sun_position		= vostok::math::float3( 0.0f, 0.0f, survarium::s_camera_far_plane );
	environment_far_fog_color		= vostok::math::float3( 1.0f, 1.0f, 1.0f );
	environment_ambient_color		= vostok::math::float3( 0.15f, 0.15f, 0.15f );
	environment_sun_color			= vostok::math::float3( 0.87f, 0.8f, 0.8f );
	environment_far_fog_distance	= 10000.0f;
	environment_near_fog_distance	= 0.0f;
	environment_shadow_transparency	= 0.5f;
	environment_fog_alpha			= 1.0f;
	aberration_max_variance			= 2.0f;
	aberration_min_variance			= 0.125f;
	aberration_power				= 1.0f;
	environment_skylight_lower_color = vostok::math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
	environment_skylight_upper_color = vostok::math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
	god_rays_color_0				= vostok::math::float4( 1.0f, 1.0f, 1.0f, 1.0f );
	god_rays_color_1				= vostok::math::float4( 1.0f, 1.0f, 1.0f, 1.0f );
	god_rays_color_blend_power		= 1.0f;
	god_rays_intensity				= 1.0f;
	god_rays_attenuation_power		= 0.5f;
	environment_rain_angle_x		= 0.0f;
	environment_rain_angle_y		= 0.0f;
	environment_rain_density		= 1.0f;
	environment_rain_speed			= 1.0f;
	environment_use_rain			= false;
	environment_skylight_parameters = vostok::math::float4( 2.0f, 2.0f, 0.0f, 0.0f );
	adaptation_speed				= 1.0f;
	tonemap_bright_threshold		= 0.05f;
	tonemap_median					= 0.5f;
	tonemap_darkness_threshold		= 0.0f;
	tonemap_middle_gray				= 0.5f;
	wind_direction					= vostok::math::float3( 1.0f, 0.0f, 1.0f );
	wind_strength					= 0.25f;
	use_color_grading_lut			= true;
	use_environment_skycolor		= false;
	color_grading_texture			= resource_manager::ref().get_color_grading_base_lut();
	lens_flares_mask_texture		= 0;
	sky_clouds_texture				= 0;
	sun_moon_texture				= 0;
	for ( u32 i = 0; i < 6; ++i )
		environment_skycolor[i]		= vostok::math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
	environment_suncolor			= vostok::math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
	sun_moon_color					= vostok::math::float3( 1.0f, 1.0f, 1.0f );
	sun_moon_billboard_scale		= 1.0f;
	use_sun_moon_atmosphere_illumination = true;
	sky_clouds_u_tile				= 1.0f;
	sky_clouds_color				= vostok::math::float4( 1.0f, 1.0f, 1.0f, 1.0f );
	sky_clouds_fog_power			= 1.0f;
	sky_clouds_fog_up_limit			= 0.25f;
	sky_clouds_rotation				= 0.0f;
	sky_clouds_blend_mode			= 0;
	filmic_tonemap_shoulder_strength = 0.22f;
	filmic_tonemap_linear_strength	= 0.3f;
	filmic_tonemap_linear_angle		= 0.1f;
	filmic_tonemap_toe_strength		= 0.2f;
	filmic_tonemap_toe_numerator	= 0.01f;
	filmic_tonemap_toe_denominator	= 0.3f;
	filmic_tonemap_liner_white		= 11.2f;
	vignette_power					= 1.0f;
	use_image_space_reflections		= true;
	skylight_color					= vostok::math::float3( 0.5f, 0.7f, 1.0f );
	skylight_lower					= 0.0f;
	skylight_upper					= 50.0f;
	skylight_power					= 1.0f;
	skylight_multiplier				= 1.0f;
	use_dynamic_lens_flares			= false;
	environment_probes_diffuse_instensity_multiplier = 1.0f;
	environment_probes_specular_instensity_multiplier = 1.0f;
	environment_rain_wet_intensity	= 1.0f;
	atmosphere_kresun_multiplier	= 1.0f;
	atmosphere_kmesun_multiplier	= 1.0f;
	atmosphere_kr4pi_multiplier		= 1.0f;
	atmosphere_km4pi_multiplier		= 1.0f;
	atmosphere_params_changed		= true;
	dof_blur_kernel					= 1;
	environment_rain_num_cones		= 16;
	environment_rain_u_scale		= 1.0f;
	environment_rain_v_scale		= 1.0f;
	environment_rain_random_rotaion_speed = 0.2f;
	environment_rain_random_rotaion_radius = 0.75f;
	environment_rain_random_base_offset = 0.5f;
	environment_rain_radius_scale	= 1.1f;
	environment_rain_start_cone_index = 5;
	lens_flares_multiplier			= 1.0f;
	atmosphere_inscatter_distance	= 1000.0f;
	atmosphere_inscatter_power		= 0.125f;
	use_atmosphere_inscattering_on_geometry = true;
	use_aberration					= true;
}

} // namespace render
} // namespace vostok
