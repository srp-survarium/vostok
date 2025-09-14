////////////////////////////////////////////////////////////////////////////
//	Created		: 17.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_PARALLEL_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_PARALLEL_LIGHT_H_INCLUDED

#include "gbuffer.h"			// for g_data
#include "common_functions.h"	// for eye_position constant
#include "light.h"				// for pixel shader input data
#include "directional_light.h"	// for light_direction
#include "colored_light.h"		// for light_color
#include "brdf.h"				// for brdf function

light_factors parallel_light_impl	(
		float3 surface_position,
		float3 surface_normal,
		float3 light_direction,
		float2 surface_roughness,
		float translucency,
		float diffuse_power,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	float3 view_direction		= normalize( mul(m_V, float4(eye_position.xyz, 1)) - surface_position );
	return
		brdf(
			-light_direction,
			view_direction,
			surface_normal,
			surface_roughness,
			texture_coordinates,
			material_parameters
		);
}

light_factors parallel_light		( g_data surface, float2 texture_coordinates )
{
	float4 material_parameters		= gbuffer_read_material_parameters(texture_coordinates);
	return
		parallel_light_impl(
			surface.position,
			surface.normal,
			light_direction,
			material_parameters.zw,
			surface.sh_param,		// translucency
			surface.diffuse_power,
			texture_coordinates,
			material_parameters
		);
}

#endif // #ifndef SHADERS_SM_4_0_PARALLEL_LIGHT_H_INCLUDED
