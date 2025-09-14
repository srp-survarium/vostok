////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SKELETON_MESH_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_SKELETON_MESH_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

#define XRAY_USE_SOFTWARE_SKINNING	0

#if XRAY_USE_SOFTWARE_SKINNING
	struct vertex_input_struct {
		float3	position		: POSITION;
		float4	normal			: NORMAL;
		float4	tangent			: TANGENT;
		float4	binormal		: BINORMAL;
		float2	tc				: TEXCOORD0;
	}; // struct vertex_input_struct
#else // #if XRAY_USE_SOFTWARE_SKINNING
	struct vertex_input_struct {
		float3	position		: POSITION;
		uint4	bones_indices	: BLENDINDICES;
		float3	bones_weights	: BLENDWEIGHT;
		float4	normal			: NORMAL;
		float4	tangent			: TANGENT;
		float4	binormal		: BINORMAL;
		float2	tc				: TEXCOORD0;
	}; // struct vertex_input_struct
#endif // #if XRAY_USE_SOFTWARE_SKINNING

struct vertex_output_struct
{	
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	normal				: NORMAL;
	float2	tc					: TEXCOORD1;
	float3	tbn_x				: TEXCOORD2;
	float3	tbn_y				: TEXCOORD3;
	float3	tbn_z				: TEXCOORD4;
};

material_parameters get_material_parameters(vertex_output_struct input)
{
	material_parameters 				parameters;
	clear_material_parameters			(parameters);
	
	parameters.world_position			= input.position;
	parameters.world_view_position 		= float4(mul(m_V, float4( input.position, 1.f)), 1.0f);
	parameters.world_view_proj_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	parameters.tc 						= input.tc;
	parameters.detail_tc 				= input.tc;
	parameters.normal 					= normalize(input.normal); // ?
	
	parameters.tangent_to_viewspace 	= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return parameters;
}

#if !XRAY_USE_SOFTWARE_SKINNING
	uniform float4x4 bones_matrices[64];
#endif // #if XRAY_USE_SOFTWARE_SKINNING

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 skinned_position	= input.position;
	float3 skinned_normal	= unpack_bx4( unpack_D3DCOLOR( input.normal.xyz));
	float3 skinned_tangent	= unpack_bx4( unpack_D3DCOLOR( input.tangent.xyz));
	float3 skinned_binormal	= unpack_bx4( unpack_D3DCOLOR( input.binormal.xyz));

#if !XRAY_USE_SOFTWARE_SKINNING
	float4x4 m0			= bones_matrices[ input.bones_indices[0] ];
	float4x4 m1			= bones_matrices[ input.bones_indices[1] ];
	float4x4 m2			= bones_matrices[ input.bones_indices[2] ];
	float4x4 m3			= bones_matrices[ input.bones_indices[3] ];

	float fourth_weight	= 1.f - input.bones_weights[0] - input.bones_weights[1] - input.bones_weights[2];

	skinned_position	=
		mul( m0, float4(skinned_position, 1.f) ).xyz * input.bones_weights[0] +
		mul( m1, float4(skinned_position, 1.f) ).xyz * input.bones_weights[1] +
		mul( m2, float4(skinned_position, 1.f) ).xyz * input.bones_weights[2] +
		mul( m3, float4(skinned_position, 1.f) ).xyz * fourth_weight;

	skinned_normal		=
		normalize(
			mul( m0, float4(skinned_normal, 0.f) ).xyz * input.bones_weights[0] +
			mul( m1, float4(skinned_normal, 0.f) ).xyz * input.bones_weights[1] +
			mul( m2, float4(skinned_normal, 0.f) ).xyz * input.bones_weights[2] +
			mul( m3, float4(skinned_normal, 0.f) ).xyz * fourth_weight
		);

	skinned_tangent		=
		normalize(
			mul( m0, float4(skinned_tangent, 0.f) ).xyz * input.bones_weights[0] +
			mul( m1, float4(skinned_tangent, 0.f) ).xyz * input.bones_weights[1] +
			mul( m2, float4(skinned_tangent, 0.f) ).xyz * input.bones_weights[2] +
			mul( m3, float4(skinned_tangent, 0.f) ).xyz * fourth_weight
		);

	skinned_binormal		=
		normalize(
			mul( m0, float4(skinned_binormal, 0.f) ).xyz * input.bones_weights[0] +
			mul( m1, float4(skinned_binormal, 0.f) ).xyz * input.bones_weights[1] +
			mul( m2, float4(skinned_binormal, 0.f) ).xyz * input.bones_weights[2] +
			mul( m3, float4(skinned_binormal, 0.f) ).xyz * fourth_weight
		);
#endif // #if XRAY_USE_SOFTWARE_SKINNING
	
	output.position		= mul(m_W,  float4( skinned_position, 1.f));
	output.normal		= mul((float3x3)m_WV,	skinned_normal);
	output.tc 			= input.tc;
	
	vsf_tangent_space_transform	(skinned_normal, skinned_tangent, skinned_binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.homogeneous_position	= mul(m_WVP,  float4( skinned_position, 1.f));
}

#endif // #ifndef SHADERS_SM_4_0_SKELETON_MESH_VERTEX_INPUT_H_INCLUDED
