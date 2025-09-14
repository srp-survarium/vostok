////////////////////////////////////////////////////////////////////////////
//	Created		: 09.03.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_POST_PROCESS_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_POST_PROCESS_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float4 position 			: POSITIONT;
	float3 eye_ray 				: POSITION;
	float2 uv					: TEXCOORD0;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float2 	uv					: TEXCOORD0;
	float3 	eye_ray				: TEXCOORD1;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.eye_ray 				= input.eye_ray;	
	output.uv 					= input.uv;
	
	output.homogeneous_position = float4(
		input.position.x * screen_res.z * 2.0f - 1.0f, 
		-1.0f * (input.position.y * screen_res.w * 2.0f - 1.0f), 
		input.position.zw
	);
}

material_parameters get_material_parameters(inout vertex_output_struct input)
{
	material_parameters 		parameters;
	clear_material_parameters	(parameters);
	parameters.tc 				= input.uv;
	
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_POST_PROCESS_VERTEX_INPUT_H_INCLUDED