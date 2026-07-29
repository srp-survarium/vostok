////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_POST_PROCESS_PARAMETERS_H_INCLUDED
#define RENDER_POST_PROCESS_PARAMETERS_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct post_process_parameters {
										post_process_parameters	( );
	inline								~post_process_parameters( ) { /* no source */ }

	inline	post_process_parameters&	operator=				( post_process_parameters const& __that ) { /* no source */ }

	/* 0x0000 */	float3				dof_height_lights;
	/* 0x000c */	float				dof_focus_power;
	/* 0x0010 */	float				dof_focus_region;
	/* 0x0014 */	float				dof_focus_distance;
	/* 0x0018 */	float				dof_far_blur_amount;
	/* 0x001c */	float				dof_near_blur_amount;
	/* 0x0020 */	bool				enable_dof;
	/* 0x0021 */	bool				use_bokeh_dof;
	/* 0x0024 */	float				bokeh_dof_radius;
	/* 0x0028 */	float				bokeh_dof_density;
	/* 0x002c */	bool				use_bokeh_image;
	/* 0x002d */	bool				use_image_grain;
	/* 0x0030 */	float				image_grain_scale;
	/* 0x0034 */	u32					image_grain_update_frequency;
	/* 0x0038 */	float				bloom_scale;
	/* 0x003c */	float				bloom_max_color;
	/* 0x0040 */	u32					blur_kernel;
	/* 0x0044 */	float				frame_dark_desaturation_limit;
	/* 0x0048 */	float3				bloom_halo_color;
	/* 0x0054 */	bool				enable_bloom;
	/* 0x0055 */	bool				enable_ssao;
	/* 0x0058 */	float				ssao_saturation;
	/* 0x005c */	float				ssao_radius_scale;
	/* 0x0060 */	float				frame_desaturation;
	/* 0x0064 */	float3				frame_height_lights;
	/* 0x0070 */	float3				frame_mid_tones;
	/* 0x007c */	float3				frame_shadows;
	/* 0x0088 */	float3				frame_fade_color;
	/* 0x0094 */	float				frame_fade_amount;
	/* 0x0098 */	bool				enable_scene;
	/* 0x0099 */	bool				enable_advanced_bloom;
	/* 0x009c */	float3				environment_sun_position;
	/* 0x00a8 */	float3				environment_far_fog_color;
	/* 0x00b4 */	float3				environment_ambient_color;
	/* 0x00c0 */	float3				environment_sun_color;
	/* 0x00cc */	float				environment_far_fog_distance;
	/* 0x00d0 */	float				environment_near_fog_distance;
	/* 0x00d4 */	float				environment_shadow_transparency;
	/* 0x00d8 */	float				environment_fog_alpha;
	/* 0x00dc */	float				aberration_max_variance;
	/* 0x00e0 */	float				aberration_min_variance;
	/* 0x00e4 */	float				aberration_power;
	/* 0x00e8 */	float4				environment_skylight_lower_color;
	/* 0x00f8 */	float4				environment_skylight_upper_color;
	/* 0x0108 */	float4				god_rays_color_0;
	/* 0x0118 */	float4				god_rays_color_1;
	/* 0x0128 */	float				god_rays_color_blend_power;
	/* 0x012c */	float				god_rays_intensity;
	/* 0x0130 */	float				god_rays_attenuation_power;
	/* 0x0134 */	float				environment_rain_angle_x;
	/* 0x0138 */	float				environment_rain_angle_y;
	/* 0x013c */	float				environment_rain_density;
	/* 0x0140 */	float				environment_rain_speed;
	/* 0x0144 */	bool				environment_use_rain;
	/* 0x0148 */	float4				environment_skylight_parameters;
	/* 0x0158 */	float				adaptation_speed;
	/* 0x015c */	float				tonemap_bright_threshold;
	/* 0x0160 */	float				tonemap_median;
	/* 0x0164 */	float				tonemap_darkness_threshold;
	/* 0x0168 */	float				tonemap_middle_gray;
	/* 0x016c */	float3				wind_direction;
	/* 0x0178 */	float				wind_strength;
	/* 0x017c */	bool				use_color_grading_lut;
	/* 0x0180 */	float4				environment_skycolor[6];
	/* 0x01e0 */	float4				environment_suncolor;
	/* 0x01f0 */	bool				use_environment_skycolor;
	/* 0x01f4 */	res_texture_ptr		color_grading_texture;
	/* 0x01f8 */	res_texture_ptr		lens_flares_mask_texture;
	/* 0x01fc */	res_texture_ptr		sky_clouds_texture;
	/* 0x0200 */	res_texture_ptr		sun_moon_texture;
	/* 0x0204 */	float3				sun_moon_color;
	/* 0x0210 */	float				sun_moon_billboard_scale;
	/* 0x0214 */	bool				use_sun_moon_atmosphere_illumination;
	/* 0x0218 */	float				sky_clouds_u_tile;
	/* 0x021c */	float4				sky_clouds_color;
	/* 0x022c */	float				sky_clouds_fog_power;
	/* 0x0230 */	float				sky_clouds_fog_up_limit;
	/* 0x0234 */	float				sky_clouds_rotation;
	/* 0x0238 */	u32					sky_clouds_blend_mode;
	/* 0x023c */	float				filmic_tonemap_shoulder_strength;
	/* 0x0240 */	float				filmic_tonemap_linear_strength;
	/* 0x0244 */	float				filmic_tonemap_linear_angle;
	/* 0x0248 */	float				filmic_tonemap_toe_strength;
	/* 0x024c */	float				filmic_tonemap_toe_numerator;
	/* 0x0250 */	float				filmic_tonemap_toe_denominator;
	/* 0x0254 */	float				filmic_tonemap_liner_white;
	/* 0x0258 */	float				vignette_power;
	/* 0x025c */	bool				use_image_space_reflections;
	/* 0x0260 */	float3				skylight_color;
	/* 0x026c */	float				skylight_lower;
	/* 0x0270 */	float				skylight_upper;
	/* 0x0274 */	float				skylight_power;
	/* 0x0278 */	float				skylight_multiplier;
	/* 0x027c */	bool				use_dynamic_lens_flares;
	/* 0x0280 */	float				environment_probes_diffuse_instensity_multiplier;
	/* 0x0284 */	float				environment_probes_specular_instensity_multiplier;
	/* 0x0288 */	float				environment_rain_wet_intensity;
	/* 0x028c */	float				atmosphere_kresun_multiplier;
	/* 0x0290 */	float				atmosphere_kmesun_multiplier;
	/* 0x0294 */	float				atmosphere_kr4pi_multiplier;
	/* 0x0298 */	float				atmosphere_km4pi_multiplier;
	/* 0x029c */	bool				atmosphere_params_changed;
	/* 0x02a0 */	u32					dof_blur_kernel;
	/* 0x02a4 */	u32					environment_rain_num_cones;
	/* 0x02a8 */	float				environment_rain_u_scale;
	/* 0x02ac */	float				environment_rain_v_scale;
	/* 0x02b0 */	float				environment_rain_random_rotaion_speed;
	/* 0x02b4 */	float				environment_rain_random_rotaion_radius;
	/* 0x02b8 */	float				environment_rain_random_base_offset;
	/* 0x02bc */	float				environment_rain_radius_scale;
	/* 0x02c0 */	u32					environment_rain_start_cone_index;
	/* 0x02c4 */	float				lens_flares_multiplier;
	/* 0x02c8 */	float				atmosphere_inscatter_distance;
	/* 0x02cc */	float				atmosphere_inscatter_power;
	/* 0x02d0 */	bool				use_atmosphere_inscattering_on_geometry;
	/* 0x02d1 */	bool				use_aberration;
}; // struct post_process_parameters

STATIC_SIZE_ASSERT(post_process_parameters, 0x2D4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_POST_PROCESS_PARAMETERS_H_INCLUDED
