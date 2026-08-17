////////////////////////////////////////////////////////////////////////////
//	Created		: 03.11.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
//
//	Reconstructed 2026-08-17: the shipped blob is byte-identical to the
//	proven clouds.vs blob, so the source is the same transform under this
//	name.
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_input_struct
{
	float4 position				: POSITION;
	float2 uv					: TEXCOORD0;
};

struct vertex_output_struct
{
	float4 hposition			: SV_Position;
	float3 world_position		: TEXCOORD0;
	float3 object_position		: TEXCOORD1;
	float2 uv					: TEXCOORD2;
	float  unused				: TEXCOORD3;
};

void main(in vertex_input_struct 	input,
		  out vertex_output_struct 	output)
{
	float4 world_position		=	float4( mul(m_W, float4(input.position.xyz, 1.0f)), 1.0f);

	output.hposition 			=	mul(m_VP, world_position);
	output.world_position 		=	world_position.xyz;
	output.object_position		=	input.position.xyz;
	output.uv					=	input.uv;
}
