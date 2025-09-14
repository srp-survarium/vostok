////////////////////////////////////////////////////////////////////////////
//	Created		: 20.04.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPHERE_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_SPHERE_LIGHT_H_INCLUDED

#include "point_light.h"
#include "light_functions.h"

uniform float								light_sphere_radius;

float3 nearest_point_on_sphere				( inout float3 surface_position )
{
	float3 light_to_surface					= surface_position - light_position;
	float light_to_surface_length			= length( light_to_surface );
	float3 to_surface_offset				= light_to_surface*(light_sphere_radius/light_to_surface_length);
	surface_position						= light_position + light_to_surface*(max( light_to_surface_length, light_sphere_radius + 0.001f )/light_to_surface_length);
	return									light_position + to_surface_offset;
}

light_factors sphere_light					( g_data surface, float2 texture_coordinates )
{
	float3 surface_position					= surface.position;
	float3 light_position					= nearest_point_on_sphere( surface_position );
	float4 material_parameters				= gbuffer_read_material_parameters(texture_coordinates);
	return
		point_light_impl(
			surface_position,
			surface.normal,
			light_position,
			light_range,
			material_parameters.zw,
			surface.sh_param,				// translucency
			surface.diffuse_power,
			texture_coordinates,
			material_parameters
		);
}

#endif // #ifndef SHADERS_SM_4_0_SPHERE_LIGHT_H_INCLUDED