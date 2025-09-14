////////////////////////////////////////////////////////////////////////////
//	Created		: 20.04.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_PLANE_SPOT_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_PLANE_SPOT_LIGHT_H_INCLUDED

#include "spot_light.h"
#include "light_functions.h"

//uniform float4x4						light_local_to_world;

float3 nearest_point_on_plane			( inout float3 surface_position )
{
	float3 dot_products					= mul( light_local_to_world, float4( surface_position - light_position, 0.f) ).xyz;
	float3 squared_lengths				= float3(
		squared_length( light_local_to_world[0].xyz ),
		squared_length( light_local_to_world[1].xyz ),
		squared_length( light_local_to_world[2].xyz )
	);
	float3 nearest_projection_value		= nearest_point_on_segment( dot_products / squared_lengths, float3( -1.f, 0.f, -1.f ), float3( 1.f, 0.f, 1.f ) );
	return								mul( float4(nearest_projection_value, 1.f), light_local_to_world ).xyz;
}

light_factors plane_spot_light			( g_data surface, float2 texture_coordinates )
{
	float3 surface_position		= surface.position;
	float3 light_position		= nearest_point_on_plane( surface_position );
	float4 material_parameters	= gbuffer_read_material_parameters(texture_coordinates);
	light_factors result		= 
		spot_light_impl(
			surface_position,
			surface.normal,
			light_position,
			normalize( mul( float4( 0.f, -1.f, 0.f, 0.f), light_local_to_world).xyz ),
			light_spot_falloff,
			light_range,
			material_parameters.zw,
			surface.sh_param,		// translucency
			surface.diffuse_power,
			texture_coordinates,
			material_parameters
		);
		
	float4 shadow_factor 		= get_spotlight_shadow_factor( surface.position.xyz, m_V2W );
	result.diffuse 				*= result.diffuse >= 0.0f ? shadow_factor.x : 1.0f;
	result.specular 			*= shadow_factor.x;
	
	return result;
}

#endif // #ifndef SHADERS_SM_4_0_PLANE_SPOT_LIGHT_H_INCLUDED