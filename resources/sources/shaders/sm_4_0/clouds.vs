////////////////////////////////////////////////////////////////////////////
//	Created		: 03.11.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_input_struct
{
	float4 position				: POSITION;
};

struct vertex_output_struct
{
	float4 hposition			: SV_Position;
	float3 world_position		: TEXCOORD0;
};

void main(in vertex_input_struct 	input,
		  out vertex_output_struct 	output)
{
	output.world_position 		=	input.position.xyz;
	output.hposition 			=	mul(m_VP, float4(input.position.xyz, 1.0f));
}















