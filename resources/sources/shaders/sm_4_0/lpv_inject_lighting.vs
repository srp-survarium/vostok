////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_input_struct
{
	float2 position		: TEXCOORD0;
	uint vertex_id 		: SV_VertexID; // use vertex id or pass uv?
};

struct vertex_output_struct
{
	float4 position		: POSITION; // 2D slice vertex coordinates in homogenous clip space
	float3 normal 		: NORMAL;
	float3 color 		: COLOR;
	float3 world_position	: TEXCOORD0;
};

int rsm_size;

Texture2D t_lpv_rsm_albedo;
Texture2D t_lpv_rsm_normal;
Texture2D t_lpv_rsm_position;

float4 	grid_origin_and_inv_grid_scale;

float3 get_grid_position_normalized(float3 world_pos)
{
	return (world_pos.xyz - grid_origin_and_inv_grid_scale.xyz) * grid_origin_and_inv_grid_scale.w;
}

void main(in vertex_input_struct input,
		  out vertex_output_struct output)
{
	float2 uv				=	input.position;
	
	float3 rsm_color 		= 	t_lpv_rsm_albedo.SampleLevel(s_accumulator, uv, 0).xyz;
	float3 rsm_normal 		= 	t_lpv_rsm_normal.SampleLevel(s_accumulator, uv, 0).xyz;
	float3 rsm_position 	=	t_lpv_rsm_position.SampleLevel(s_accumulator, uv, 0).xyz;
	
	float3 norm_position 	= 	get_grid_position_normalized(rsm_position);
	
	// If outside of the radiance volume
	
	norm_position.x 		*= 	(any(norm_position < 0.0f) || any(norm_position > 1.0f)) ? 2.0f * 1000000.0f: 1.0f;
	
	output.world_position	=	rsm_position;
	output.position 		=	float4(norm_position, 1.0f);
	output.normal 			=	rsm_normal;
	output.color 			=	rsm_color;
}















