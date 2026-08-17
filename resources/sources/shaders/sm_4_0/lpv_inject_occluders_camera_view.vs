////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 against the shipped v0.100b blob
//	(shaders/sm_4_0/lpv_inject_occluders_camera_view.vs, plain permutation).
//	The recovered source had drifted: main accumulated a dead 4x4 loop of
//	identical samples and divided by 16 (temps 6); the ship compile samples
//	each downsampled g-buffer target once at screen_uv + screen_res.zw and
//	uses the raw values (temps 2). Byte-identity is the acceptance test:
//	python3 -m vostok.shaders roundtrip lpv_inject_occluders_camera_view.vs
////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "gbuffer.h"

struct vertex_input_struct
{
	float2 position		: TEXCOORD0;
	uint vertex_id 		: SV_VertexID; // use vertex id or pass uv?
};

struct vertex_output_struct
{
	float4 position			: POSITION; // 2D slice vertex coordinates in homogenous clip space
	float3 normal 			: NORMAL;
	float3 color 			: COLOR;
	float3 world_position 	: TEXCOORD0;
};

Texture2D t_gbuffer_position_downsampled;
Texture2D t_gbuffer_normal_downsampled;

float3 	grid_origin;
float 	grid_cell_size;
float 	num_grid_cells;

float3 	s_eye_ray_corner;


float3 get_cell_world_position(float3 world_pos)
{
	return floor(world_pos.xyz / grid_cell_size) * grid_cell_size + 0.5f / num_grid_cells;
}

float3 get_grid_position(float3 world_pos)
{
	// TODO: optimize, pass calculated values
	float3 offset = 0.0f * 0.5f * 1.0f * (16.0f / 32.0f);
	return floor((world_pos.xyz - (grid_origin + offset)) / grid_cell_size) / num_grid_cells;
}

float3 get_grid_position_normalized(float3 world_pos)
{
	// TODO: optimize, pass calculated values
	float3 offset = 0.0f * 0.5f * 1.0f * (16.0f / 32.0f);
	return (world_pos.xyz - (grid_origin + offset)) / grid_cell_size / num_grid_cells;
}

float3 read_screen_normal( float2 uv)
{
	float3 normal			= 	0;
	float2 temp_value 		= 	0;

	gbuffer_read_rt_normal	(uv, normal, temp_value.x, temp_value.y);

	return normalize(mul(m_V2W, float4(normal, 0.0f)).xyz);
}

float3 read_screen_vs_normal(float2 uv)
{
	float3 normal			= 	0;
	float2 temp_value 		= 	0;

	gbuffer_read_rt_normal	(uv, normal, temp_value.x, temp_value.y);

	return normal;
}

float3 read_screen_position(float2 uv)
{
	float2 temp_value 		= 	0;
	float3 position			=	0;

	float3 eye_ray			=	0;
	eye_ray.xy				= 	s_eye_ray_corner.xy * (1.0f - uv.xy * 2.0f);
	eye_ray.z				= 	s_eye_ray_corner.z;

	gbuffer_read_rt_position(uv, eye_ray, position, temp_value.x, temp_value.y);

	return mul(m_V2W, float4(position, 1.0f)).xyz;
}

void main(in vertex_input_struct input,
		  out vertex_output_struct output,
		  uint vertex_id : SV_VertexID)
{
	float2 screen_uv		=	input.position;

	float2 uv_offset		= 	1.0f * screen_res.zw;

	float3 pos				= 	t_gbuffer_position_downsampled.SampleLevel(s_accumulator, screen_uv + uv_offset, 0).xyz;
	float3 norm 			= 	t_gbuffer_normal_downsampled.SampleLevel(s_accumulator, screen_uv + uv_offset, 0).xyz;

	output.world_position	=	pos;

	float3 norm_position	=	get_grid_position_normalized(pos);

	output.position 		=	float4(norm_position, 1.0f);

	output.normal 			=	norm;
	output.color 			=	0;
}
