////////////////////////////////////////////////////////////////////////////
//	Created		: 17.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_CAPSULE_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_CAPSULE_LIGHT_H_INCLUDED

#include "point_light.h"
#include "directional_light.h"
#include "light_functions.h"

uniform float								light_capsule_half_width;
uniform float								light_capsule_radius;

float3 nearest_point_on_capsule				( inout float3 surface_position, float3 light_direction )
{
	float projection_length					= dot( surface_position - light_position, light_direction );
	float nearest_projection_value			= nearest_point_on_segment( projection_length, -light_capsule_half_width, +light_capsule_half_width );
	float3 nearest_position_on_projection	= light_position + nearest_projection_value * light_direction;
	float3 surface_direction				= normalize( surface_position - nearest_position_on_projection );
	float3 nearest_position					= nearest_position_on_projection + surface_direction*light_capsule_radius;

	// the next 2 lines of code are optional and may be removed
	// they remove artefacts, when surface is inside light source
	float distance_to_surface				= max( length(surface_position - nearest_position_on_projection), light_capsule_radius + .001f );
	surface_position						= nearest_position_on_projection + surface_direction*distance_to_surface;

	return									nearest_position;
}

light_factors capsule_light					( g_data surface, float2 texture_coordinates )
{
	float3 surface_position					= surface.position;
	float3 light_position					= nearest_point_on_capsule( surface_position, light_direction );
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

#endif // #ifndef SHADERS_SM_4_0_CAPSULE_LIGHT_H_INCLUDED