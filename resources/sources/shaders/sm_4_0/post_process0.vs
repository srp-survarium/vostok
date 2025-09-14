////////////////////////////////////////////////////////////////////////////
//	Created		: 06.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "common.h"

uniform float4 screen_res; // Screen resolution (x-Width,y-Height, zw - 1/resolution)

void main ( in v_post_process input, 
			out v2p_post_process output 
	)
{
	output.homogeneous_position 	= float4(
		input.position.x * screen_res.z * 2.0f - 1.0f, 
		-1.0f * (input.position.y * screen_res.w * 2.0f - 1.0f), 
		input.position.zw
	);
	output.eye_ray 					= input.eye_ray;
	output.uv 						= input.uv;
}