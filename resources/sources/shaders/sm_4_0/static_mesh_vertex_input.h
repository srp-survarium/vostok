////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_STATIC_MESH_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_STATIC_MESH_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float3	position			: POSITION;
	float3	normal				: NORMAL;
	float3	tangent				: TANGENT;
	float3	binormal			: BINORMAL;
	float2	tc					: TEXCOORD0;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	normal				: NORMAL; 	// TODO: Remove normal.
	float2	tc					: TEXCOORD1;
	float3	tbn_x				: TEXCOORD2;
	float3	tbn_y				: TEXCOORD3;
	float3	tbn_z				: TEXCOORD4;
};

void add_base_wind_motion(inout vertex_input_struct input)
{
	float dist  				= input.position.y * length(input.position);
	float s, c;
	
	float3 object_pos			=	float3(m_W[0][3], m_W[1][3], m_W[2][3]);
	
	const float period			=	0.2f;
	const float movement_mult 	=	0.0025f;
	
	s							=	sin(scene_time + period * object_pos.x);
	c							=	cos(scene_time + period * object_pos.z);
	
	input.position.x 			+=  movement_mult * dist * s;
	input.position.z 			+=  movement_mult * dist * c;
}

void add_leafmesh_wind_motion(inout vertex_input_struct input)
{
	add_base_wind_motion		(input);
	
	float dist  				= 0.0125f * length(input.position);
	
	const float c_scene_time 	= (scene_time + length(float3(m_W[0][3], m_W[1][3], m_W[2][3]))) * 0.25f;
	
	float s, c;
	sincos						(c_scene_time, s, c);
	
	float3 sp 					= float3(c, s, 1.0f - s) * pow(sin(input.position*5), 3.0f);
	
	sp.y 						*= 1.0f;
	sp.xz 						*= 0.5f;
	
	input.position 				+= dist * s * c * c + s * s * c * sp;
}

void add_wind_motion(inout vertex_input_struct input)
{
#if CONFIG_WIND_MOTION==1
	add_base_wind_motion		(input);
#elif CONFIG_WIND_MOTION==2
	add_leafmesh_wind_motion	(input);
#endif
}

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	add_wind_motion				(input);
	
	float3 normal				= unpack_bx4( unpack_D3DCOLOR( input.normal));
	float3 tangent				= unpack_bx4( unpack_D3DCOLOR( input.tangent));
	float3 binormal				= unpack_bx4( unpack_D3DCOLOR( input.binormal));
	
	output.position				= mul(m_W,  float4( input.position, 1.f));
	output.normal				= mul((float3x3)m_WV,	input.normal); // TODO: Remove normal.
	
	output.tc 					= input.tc;
	
	vsf_tangent_space_transform	(normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.homogeneous_position	= mul( m_WVP,  float4( input.position, 1.f));
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
	parameters.normal 					= normalize(input.tbn_z); // TODO: Remove normal.
	parameters.tangent_to_viewspace 	= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_STATIC_MESH_VERTEX_INPUT_H_INCLUDED
