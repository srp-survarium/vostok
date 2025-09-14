////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_GRASS_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_GRASS_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "vsf_tangent_space_transform.h"

// TODO: its a m_V2W
//float4x4 view_to_local_matrix;

#define GRASS_BILLBOARD (CONFIG_VERTEX_INPUT_TYPE==GRASSBILLBOARD_VERTEX_INPUT_TYPE)

struct vertex_input_struct
{
	float3	location			: POSITION;
	float3	offset				: TEXCOORD0; // offsets or size if billboard
	float2	rotation			: TEXCOORD1; // sin(A), cos(A) or corners if billboard
	float2	tc					: TEXCOORD2;
	float4	color				: TEXCOORD3;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	normal				: NORMAL;
	float2	tc					: TEXCOORD1;
	float3	tbn_x				: TEXCOORD2;
	float3	tbn_y				: TEXCOORD3;
	float3	tbn_z				: TEXCOORD4;
	float4	color				: TEXCOORD5;
};

static const float3 wind_direction = float3(1.0f, 0.0f, 1.0f);

void add_grass_wind_motion(in float3 location, inout float3 position)
{
	float dist  				= position.y * length(position);
	float s, c;
	
	float time 					= scene_time*2;
	sincos						(time, s, c);
	
	float3 sp 					= sin(0.2f*location);
	
	float3 global_offset 		= (0.1+0.8*sin(time)) * wind_direction * 0.15f;
	//float3 global_offset 		= sin(time+0.1*location.x) * wind_direction * 0.15f;
	float3 local_offset  		= float3(0.2f*s, 0, 0.2f*c) * s * c * sp;
	
	position 					+= dist * (0.25f*global_offset + 0.25f*local_offset);
}

void rotate_grass(in float2 rotation, inout float3 position)
{
	float3x3 rotation_matrix 	= float3x3(
		float3( rotation.y, 	0, 	rotation.x),
		float3(		 	 0, 	1, 			 0),
		float3(-rotation.x, 	0,  rotation.y)
		);
	
	position					= mul(rotation_matrix, position);
}

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 location				= input.location;
	
	output.tc 					= input.tc;
	
#if GRASS_BILLBOARD
	float3 camera_z 			= mul(m_V2W, float4(0,0,1,0)).xyz;
	float3 up 					= float3(0,1,0);
	float3 right 				= normalize(cross(up,camera_z).xyz);
	float3 offsets				= input.offset.y * (1.0f-input.rotation.y) * up + input.offset.x * (input.rotation.x - 0.5f) * right;
#else
	float3 offsets		 		= input.offset;
	// TODO: remove this rotation! Rotate when generate grass.
	rotate_grass		 		(input.rotation, offsets);
#endif // GRASS_BILLBOARD
	add_grass_wind_motion		(location, offsets);
	
	output.position				= mul(m_W,  float4( location + offsets, 1.f));
	output.normal				= mul((float3x3)m_WV,	float3(0,1,0));
	
	output.color				= clamp(offsets.y*8,0,1);
	output.color.a				= input.color.a;
	
	vsf_tangent_space_transform	(float3(0,1,0), float3(1,0,0), float3(0,0,1), output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.homogeneous_position = mul(m_WVP, float4( location + offsets, 1.f));
}

material_parameters get_material_parameters(inout vertex_output_struct input)
{
	material_parameters 				parameters;
	clear_material_parameters			(parameters);
	
	parameters.world_position			= input.position;
	parameters.world_view_position 		= float4(mul(m_V, float4( input.position, 1.f)), 1.0f);
	parameters.world_view_proj_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	parameters.tc 						= input.tc;
	parameters.detail_tc 				= input.tc;
	parameters.color					= input.color;
	parameters.normal 					= normalize(input.normal); // ?
	parameters.tangent_to_viewspace 	= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_GRASS_VERTEX_INPUT_H_INCLUDED