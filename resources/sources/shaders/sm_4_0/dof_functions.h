////////////////////////////////////////////////////////////////////////////
//	Created		: 21.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef	DOF_FUNCTIONS_H_INCLUDED
#define	DOF_FUNCTIONS_H_INCLUDED

float4 dof_height_lights;
float4 dof_parameters;
float4 blurriness_amount; // float4(1,1,0,0)

static const float  focus_distance 		= dof_parameters.x; 	// 0.0f
static const float  focus_region   		= dof_parameters.y; 	// 20.0f
static const float  focus_power    		= dof_parameters.z; 	// 4.0f
static const float	blur_amount_near 	= blurriness_amount.x;
static const float	blur_amount_far 	= blurriness_amount.y;

float get_blurriness(float in_depth)
{
	return min(
		(in_depth-focus_distance) >= 0.0f ? blur_amount_far : blur_amount_near,
		pow( saturate(abs(in_depth-focus_distance) / focus_region), focus_power )
	);
}

#endif	// #ifndef DOF_FUNCTIONS_H_INCLUDED