////////////////////////////////////////////////////////////////////////////
//	Created		: 04.10.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_output_struct
{
	float4 position				: POSITION;
	float4 slice_xy				: TEXCOORD0;
};

struct geometry_output_struct
{
    float4 position 			: SV_Position;
	float3 grid_position_norm	: TEXCOORD0;
    uint rt_index 				: SV_RenderTargetArrayIndex;
};

[maxvertexcount (3)]
void main(triangle vertex_output_struct input[3], inout TriangleStream<geometry_output_struct> triangle_stream)
{
	for (int i = 0; i < 3; i++)
	{
		geometry_output_struct 		output;
		output.grid_position_norm 	= 	input[i].position.xyz;
		output.rt_index				=	input[i].position.w;
		output.position        		= 	input[i].slice_xy;
		triangle_stream.Append		(output);
	}
	triangle_stream.RestartStrip	();
}

















