////////////////////////////////////////////////////////////////////////////
//	Created		: 17.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPOT_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_SPOT_LIGHT_H_INCLUDED

#include "debug.h"
#include "point_light.h"
#include "directional_light.h"
#include "spot_shadow.h"

// cos(penumbra_angle/2)
uniform float	light_spot_penumbra_half_angle_cosine;
// cos(umbra_angle/2)
uniform float	light_spot_umbra_half_angle_cosine;
// 1/( cos(umbra_angle/2) - cos(penumbra_angle/2) )
uniform float	light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine;
uniform float	light_spot_falloff;

light_factors spot_light_impl	(
		float3 surface_position,
		float3 surface_normal,
		float3 light_position,
		float3 light_direction,
		float light_spot_falloff,
		float light_range,
		float2 surface_roughness,
		float translucency,
		float diffuse_power,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	light_factors result		=
		point_light_impl(
			surface_position,
			surface_normal,
			light_position,
			light_range,
			surface_roughness,
			translucency,
			diffuse_power,
			texture_coordinates,
			material_parameters
		);

	float current_angle_cosine	= dot( normalize(surface_position - light_position), light_direction );
	
	float spot_scale_factor		= 
		current_angle_cosine >= light_spot_umbra_half_angle_cosine ?
			1.f :
			pow(
					max( current_angle_cosine - light_spot_penumbra_half_angle_cosine, 0.f )
					*
					ABS_TO_REMOVE_WARNING( light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine ),
				light_spot_falloff
			);
	
	
	result.diffuse				*= spot_scale_factor;
	result.specular				*= spot_scale_factor;
	
	return						result;
}

light_factors spot_light		( g_data surface, float2 texture_coordinates )
{
	float4 material_parameters	= gbuffer_read_material_parameters(texture_coordinates);
	light_factors result		=
		spot_light_impl(
			surface.position,
			surface.normal,
			light_position,
			light_direction,
			light_spot_falloff,
			light_range,
			material_parameters.zw,
			surface.sh_param,		// translucency
			surface.diffuse_power,
			texture_coordinates,
			material_parameters
		);
	
	float3 offset				= surface.normal * 12.0f / shadow_map_size;
	float4 shadow_factor 		= get_spotlight_shadow_factor( surface.position.xyz + offset, m_V2W );
	result.diffuse 				*= result.diffuse >= 0.0f ? shadow_factor.x : 1.0f;
	result.specular 			*= shadow_factor.x;
	
	return result;
}

#endif // #ifndef SHADERS_SM_4_0_SPOT_LIGHT_H_INCLUDED