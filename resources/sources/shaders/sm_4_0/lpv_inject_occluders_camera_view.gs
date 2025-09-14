////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_output_struct
{
	float4 position			: POSITION;
	float3 normal 			: NORMAL;
	float3 color 			: COLOR;
	float3 world_position 	: TEXCOORD0;
};

struct geometry_output_struct
{
    float4 position 	: SV_Position; // 2D slice vertex coordinates in homogenous clip space
    float3 normal 		: NORMAL;
    float3 color 		: COLOR;
    uint rt_index 		: SV_RenderTargetArrayIndex;  // used to choose the destination slice
};

float	grid_size;

[maxvertexcount (1)]
void main(point vertex_output_struct input[1], inout PointStream<geometry_output_struct> point_stream)
{
    geometry_output_struct 		output;
	
	float3 grid_pos				=	input[0].position.xyz; 
	
	float3 wp					=	input[0].world_position;
	float3 offset				=	0.5f * normalize(wp - eye_position.xyz) / grid_size;
	
	// Shift on half normal.
	//grid_pos 					+= 	-0.55f * normalize(input[0].normal) / grid_size;
	 
    output.rt_index         	= 	floor(grid_pos.z * grid_size);
	
	
	output.position        		= 	float4(
		grid_pos.x * 2.0f - 1.0f,
		(1.0f - grid_pos.y) * 2.0f - 1.0f,
		0.0f,
		1.0f
	);
	
    output.normal         		= 	input[0].normal;
    output.color         		= 	input[0].color;
	
    point_stream.Append			(output);
}