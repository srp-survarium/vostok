////////////////////////////////////////////////////////////////////////////
//	Created		: 25.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_LENSFLARES_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_LENSFLARES_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float3	position					: POSITION;
	float3	normal						: NORMAL;
	float2	tc							: TEXCOORD0;
	float3	tangent						: TANGENT;
	float3	binormal					: BINORMAL;
};

struct vertex_output_struct
{
	float4  homogeneous_position		: SV_Position;
	float3	position					: TEXCOORD0;
	float3	normal						: NORMAL;
	float2	tc							: TEXCOORD1;
	float3	tbn_x						: TEXCOORD2;
	float3	tbn_y						: TEXCOORD3;
	float3	tbn_z						: TEXCOORD4;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 normal						= unpack_bx4( unpack_D3DCOLOR( input.normal));
	float3 tangent						= unpack_bx4( unpack_D3DCOLOR( input.tangent));
	float3 binormal						= unpack_bx4( unpack_D3DCOLOR( input.binormal));
	output.position						= mul(m_W,  float4( input.position, 1.f));
	output.normal						= mul((float3x3)m_WV,	input.normal);
	output.tc 							= input.tc;
	vsf_tangent_space_transform			(normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	output.homogeneous_position			= mul(m_WVP,  float4( input.position, 1.f));
}

material_parameters get_material_parameters(inout vertex_output_struct input)
{
	material_parameters					parameters;
	clear_material_parameters			(parameters);
	
	parameters.world_position			= input.position;
	parameters.world_view_position 		= float4(mul(m_V, float4( input.position, 1.f)), 1.0f);
	parameters.world_view_proj_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	parameters.tc 						= input.tc;
	parameters.detail_tc 				= input.tc;
	parameters.normal 					= normalize(input.normal); // ?
	parameters.tangent_to_viewspace		= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_LENSFLARES_VERTEX_INPUT_H_INCLUDED