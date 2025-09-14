////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_output_struct
{
	float4 position			: POSITION; // 2D slice vertex coordinates in homogenous clip space
	float3 normal 			: NORMAL;
	float3 color 			: COLOR;
	float3 world_position	: TEXCOORD0;
};

struct geometry_output_struct
{
    float4 position 		: SV_Position; // 2D slice vertex coordinates in homogenous clip space
    float3 normal 			: NORMAL;
    float3 color 			: COLOR;
	float3 world_position	: TEXCOORD0;
    uint rt_index 			: SV_RenderTargetArrayIndex;  // used to choose the destination slice
};

float	grid_size;
float3 	light_position;

[maxvertexcount (1)]
void main(point vertex_output_struct input[1], inout PointStream<geometry_output_struct> point_stream)
{
    geometry_output_struct 		output;
	
	float3 grid_pos				=	input[0].position.xyz; 
	
	float3 to_light				=	normalize(light_position - input[0].world_position);
	
	// Shift on half normal.
	//grid_pos 					-= 	0.55f * normalize(input[0].normal) / grid_size;// - 0.5f * to_light / grid_size;
	
    output.rt_index         	= 	floor(grid_pos.z * grid_size);
	
	output.position        		= 	float4(
		grid_pos.x * 2.0f - 1.0f,
		(1.0f - grid_pos.y) * 2.0f - 1.0f,
		0.0f,
		1.0f
	);
	
	output.world_position		=	input[0].world_position;
    output.normal         		= 	input[0].normal;
    output.color         		= 	input[0].color;
	
    point_stream.Append			(output);
}