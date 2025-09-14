////////////////////////////////////////////////////////////////////////////
//	Created		: 16.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_POINT_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_POINT_LIGHT_H_INCLUDED

#include "common.h"
#include "brdf.h"
#include "gbuffer.h"
#include "light.h"
#include "positional_light.h"
#include "colored_light.h"
#include "spot_shadow.h"

//uniform float3x4 				m_V2W;

float is_shadower;

light_factors point_light_impl	(
		float3 surface_position,
		float3 surface_normal,
		float3 light_position,
		float light_range,
		float2 surface_roughness,
		float translucency,
		float diffuse_power,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	light_factors				result;
	
	float3 light_direction		= light_position - surface_position;
	
	// distance from surface to light
	float square_distance		= squared_length( light_direction );
	float distance				= sqrt( square_distance );
	light_direction				/= distance;
	
//	float3 eye_view_space		= mul(m_V, float4(eye_position.xyz, 1)).xyz;
//	float3 view_direction		= normalize( eye_view_space - surface_position );
	float3 view_direction		= normalize( -surface_position );
	
	result						= brdf( light_direction, view_direction, surface_normal, surface_roughness, texture_coordinates, material_parameters );
	
	// physical attenuation
//	float attenuation_helper	= square_distance + 0.f;//dot( float3(1.f, 0.f, 1.f), float3(square_distance, distance, 1.f ) );
//	float attenuation			= attenuation_helper > 0.f ? 1.f/attenuation_helper : 1.f;
//	attenuation				= (distance > light_range) ? 0.f : attenuation;
	
	// radial_attenuation
	float attenuation			= pow( 
		max( abs(1.f - saturate( square_distance/square(light_range) )), 0.001f ), 
		light_attenuation_power 
	);
	
	result.diffuse				*= attenuation;
	result.specular				*= attenuation;
/*	
	BRANCH
	if (is_shadower > 0.0f)
	{
		result.diffuse				= saturate(1.0f - saturate(dot(surface_normal, light_direction)));
		result.diffuse				= lerp(1.0f, result.diffuse, saturate(attenuation));
		result.specular				= result.diffuse;		
	}
*/
	return						result;
}

light_factors point_light		( g_data surface, float2 texture_coordinates )
{
	float4 material_parameters	= gbuffer_read_material_parameters(texture_coordinates);
	light_factors result		= 
		point_light_impl(
			surface.position,
			surface.normal,
			light_position,
			light_range,
			material_parameters.zw,
			surface.sh_param,	// translucency
			surface.diffuse_power,
			texture_coordinates,
			material_parameters
		);
	
	float3 offset				= surface.normal * 12.0f / shadow_map_size;
	float4 shadow_factor 		= get_pointlight_shadow_factor( surface.position.xyz + offset, m_V2W );
	result.diffuse 				*= result.diffuse >= 0.0f ? shadow_factor.x : 1.0f;
	result.specular 			*= shadow_factor.x;
	
	return						result;
}

#endif // #ifndef SHADERS_SM_4_0_POINT_LIGHT_H_INCLUDED
