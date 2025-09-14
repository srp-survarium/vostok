////////////////////////////////////////////////////////////////////////////
//	Created		: 13.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_POSTPROCESS_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_POSTPROCESS_PARAMETERS_H_INCLUDED

#if VOSTOK_PLATFORM_WINDOWS
#	include <vostok/render/core/resource_intrusive_base.h>
#	include <vostok/render/core/res_texture.h>
#endif // #if VOSTOK_PLATFORM_WINDOWS

namespace vostok {
namespace render {

#define NUM_TONEMAP_TEXTURES			9
#define NUM_HISTOGRAM_VALUES			16
#define MAX_TONEMAP_TEXTURE_DIMENSION	256
#define NUM_TONEMAP_PIXELS				(MAX_TONEMAP_TEXTURE_DIMENSION * MAX_TONEMAP_TEXTURE_DIMENSION)
// TODO: 0.5f
#define MIN_FRAME_LUMINANCE				0.0f
#define MAX_FRAME_LUMINANCE				4.0f

#if VOSTOK_PLATFORM_WINDOWS
class res_texture;
typedef intrusive_ptr<res_texture, resource_intrusive_base, threading::single_threading_policy> ref_texture;
#endif // #if VOSTOK_PLATFORM_WINDOWS

struct post_process_parameters
{
	post_process_parameters();
	
	// DOF part.
	vostok::math::float3	dof_height_lights;
	float				dof_focus_power;
	float				dof_focus_region;
	float				dof_focus_distance;
	float				dof_far_blur_amount;
	float				dof_near_blur_amount;
	bool				enable_dof;
	bool				use_bokeh_dof;
	float				bokeh_dof_radius;
	float				bokeh_dof_density;
	bool				use_bokeh_image;
	
	// Bloom part.
	float			    bloom_scale;
	float			    bloom_blurring_kernel;
	float			    bloom_blurring_intencity;
	float				bloom_max_color;
	bool				enable_bloom;
	
	// SSAO part.
	bool				enable_ssao;
	float				ssao_saturation;
	float				ssao_radius_scale;

	// Color correction part.
	float				frame_desaturation;
	vostok::math::float3	frame_height_lights;
	vostok::math::float3	frame_mid_tones;
	vostok::math::float3	frame_shadows;
	vostok::math::float3	frame_fade_color;
	float				frame_fade_amount;
	float				frame_gamma_correction_factor;
	bool				enable_scene;
	
	
	//Temporary. Environment.
	vostok::math::float3  environment_sun_position;
	vostok::math::float3  environment_far_fog_color;
	vostok::math::float3  environment_ambient_color;
	vostok::math::float3  environment_sun_color;
	
	float			  	environment_far_fog_distance;
	float			  	environment_near_fog_distance;
	float			  	environment_shadow_transparency;
	
	vostok::math::float4	environment_skylight_lower_color;
	vostok::math::float4	environment_skylight_upper_color;
	
	// upper darkness, power, lower darkness, power
	vostok::math::float4	environment_skylight_parameters;
	
	float			  	adaptation_speed;
	
	float				tonemap_bright_threshold;
	float				tonemap_median;
	float				tonemap_darkness_threshold;
	float				tonemap_middle_gray;
	
	
	// TODO: remove from post_process_parameters!
	vostok::math::float3  wind_direction;
	float			  	wind_strength;
	
	bool				use_color_grading_lut;
	
	bool				use_msaa;
	
	vostok::math::float4 environment_skycolor[6];
	bool				use_environment_skycolor;
	
#if VOSTOK_PLATFORM_WINDOWS
	ref_texture			color_grading_texture;
#endif // #if VOSTOK_PLATFORM_WINDOWS
	
}; // struct post_process_parameters

} // namespace render
} // namespace vostok

#endif // #ifndef POSTPROCESS_PARAMETERS_H_INCLUDED