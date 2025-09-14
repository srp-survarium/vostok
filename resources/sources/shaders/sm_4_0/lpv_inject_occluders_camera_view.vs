////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
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

uniform float4 screen_res;

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
    //int x = (vertex_id & 1023);
    //int y = (int)( (float)vertex_id / 1024); 
	//float fx = (float)x / 1024.0f;
	//float fy = (float)y / 1024.0f;
	//float2(fx, fy) + 0.0f / 1024.0f;//
	float2 screen_uv			=	input.position;
	//screen_uv					*=	1.05;
	
	//screen_uv = clamp(screen_uv, 0, 1);
	
	float3 world_space_normal 	= 	0.0f;
	float3 world_position 		= 	0.0f;
	
	/*
	UNROLL for (int i = 0; i <= 0; i++)
	{
		UNROLL for (int j = 0; j <= 0; j++)
		{
			float2 uv_offset				= 	2.0f * float2(i / (1.0f * 1056.0f), j / (1.0f * 959.0f));
			float3 curr_world_position 		= 	read_screen_position(screen_uv + uv_offset);
			float3 curr_world_space_normal 	= 	read_screen_normal(screen_uv + uv_offset);
			
			if (curr_world_position.z < 9000.0f)
			{
				world_position 				= max(curr_world_position, world_position);// * length(read_screen_vs_normal(screen_uv + uv_offset));
				world_space_normal 			= max(world_space_normal, curr_world_space_normal);// * length(read_screen_vs_normal(screen_uv + uv_offset));
			}
		}
	}
	
	world_space_normal 		/= 	1.0f;
	world_position 			/= 	1.0f;
	
	float3 norm_position	=	get_grid_position_normalized(world_position);
	
	norm_position.x 		*= 	(any(norm_position < 0.0f) || any(norm_position > 1.0f)) ? 2.0f * 1000000.0f : 1.0f;
	*/
	
	float3 pos = 0;
	float3 norm = 0;
	bool found = false; 
	float max_l = 1000;
	
	UNROLL for (int i = 0; i <= 3; i++)
	{
		UNROLL for (int j = 0; j <= 3; j++)
		{
			float2 uv_offset	= 	1.0f * screen_res.zw;
			
			float3 pos0			= 	t_gbuffer_position_downsampled.SampleLevel(s_accumulator, screen_uv + uv_offset, 0).xyz;
			float3 norm0 		= 	t_gbuffer_normal_downsampled.SampleLevel(s_accumulator, screen_uv + uv_offset, 0).xyz;
			
			pos					+=	pos0;
			norm				+=	norm0;
			found				=	true;
		}
	}
	
	pos						=	pos / 16.0f;
	norm					=	norm / 16.0f;
	
	
	//pos						=	get_cell_world_position(pos);
	
	output.world_position	=	pos;
	
	float4 screen_pos		=	mul(m_VP, float4(pos, 1.0f));
	screen_pos.xy			/=	screen_pos.w;
	screen_pos.y			*= 	-1.0f;
	screen_pos.xy			=	screen_pos.xy * 0.5f + 0.5f;
	screen_pos				=	clamp(screen_pos / 1.0f, 0, 1);
	
	//norm 					= 	t_gbuffer_normal_downsampled.SampleLevel(s_accumulator, screen_pos.xy, 0).xyz;
	
	float3 norm_position	=	get_grid_position_normalized(pos);
	
	//norm_position.x 		*= 	(any(norm_position < 0.0f) || any(norm_position > 1.0f)) ? 1000000.0f: 1.0f;
	output.position 		=	float4(norm_position, 1.0f);
	
	output.normal 			=	norm;
	output.color 			=	0;//pos.xyz / 500;
}















