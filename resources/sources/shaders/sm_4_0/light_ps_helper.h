////////////////////////////////////////////////////////////////////////////
//	Created		: 17.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_LIGHT_PS_HELPER_H_INCLUDED
#define SHADERS_SM_4_0_LIGHT_PS_HELPER_H_INCLUDED

#include "light_accumulator.h"
#include "gbuffer.h"
#include "gamma_correction.h"
#include "colored_light.h"

uniform float	light_diffuse_influence_factor;
uniform float	light_specular_influence_factor;

g_data get_gbuffer_data					( float3 texture_coordinates, float3 eye_ray_corner )
{
	// Compute scene texture coordinates
	float2 tc_proj		= texture_coordinates.xy / texture_coordinates.z;

	// Compute eye_ray using screen texture coordinates
	float3				eye_ray; 
	eye_ray.xy			= eye_ray_corner.xy*( 1 - tc_proj.xy*2 );
	eye_ray.z			= eye_ray_corner.z;

	return				gbuffer_read( tc_proj, eye_ray );
}

#define LIGHT_ACCUMULATOR_MAGNIFY_FACTOR	1.f

light_accumulator get_light_accumulator	( float3 light_color, light_factors light_factors )
{
	light_accumulator result;
	
	result.diffuse 						= float4( convert_to_linear_space( (light_color * max(light_factors.diffuse,  0.0f) * light_diffuse_influence_factor ) / LIGHT_ACCUMULATOR_MAGNIFY_FACTOR ) * light_intensity, 0.f );
	result.specular						= float4( ( (convert_to_linear_space(light_color) * max(light_factors.specular, 0.0f) * light_specular_influence_factor) / LIGHT_ACCUMULATOR_MAGNIFY_FACTOR ) * light_intensity, 0.f );
	
	result.diffuse.w					= light_factors.diffuse < 0.0f ? length( convert_to_linear_space( light_color * abs(light_factors.diffuse) ) * light_intensity ) : 0.0f;
	
	return								result;
}

#endif // #ifndef SHADERS_SM_4_0_LIGHT_PS_HELPER_H_INCLUDED
