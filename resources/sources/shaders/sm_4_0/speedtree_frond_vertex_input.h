////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPEEDTREE_FROND_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_SPEEDTREE_FROND_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "speedtree_common.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float3	position			: POSITION;
	float4	lod_pos_wind		: TEXCOORD0;
	float4	tc					: TEXCOORD1; // diffuse texcoords.st + frond ripple
	uint4	normal				: TEXCOORD2; // normal.xyz + amb occ
	uint4	tangent				: TEXCOORD3;
	uint4	wind_data			: TEXCOORD4; // wind data.xyzw
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

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	static const float lod 		= tree_rotation_parameter.z;
	
	float4 normal_and_occl 		= uncompress_vector4(input.normal);
	float3 normal 				= normal_and_occl.xyz;
	float3 tangent 				= uncompress_vector4(input.tangent).xyz;
	
	//normal 						= adjust_for_tree_rotation(normal, tree_rotation_parameter.xy);
	//tangent 					= adjust_for_tree_rotation(tangent, tree_rotation_parameter.xy);
	
	const float wind_scalar_mag = input.lod_pos_wind.w;
	const float4 wind_data = uncompress_wind_vector(input.wind_data, wind_scalar_mag);
	const float2 frond_ripple = input.tc.zw;
	
	#if defined(SPEEDTREE_FROND_RIPPLING) && defined(SPEEDTREE_BASIC_WIND)
		 //these ripple wind functions also handle the frond LOD lerp
		float3 position 		= ripple_frond(
			input.lod_pos_wind.xyz, 
			input.position.xyz, 
			frond_ripple, 
			normal
			).xyz;
	#else
		// LOD interpolation; fronds are shrunk & grown, depending on the LOD setting
		float3 position 		= lerp(input.lod_pos_wind.xyz, input.position.xyz, lod);
	#endif	
	
	// move the frond vertex with wind
	#ifdef SPEEDTREE_BASIC_WIND
		float3 vWindOffset;
		position 				= common_wind_motion(position, wind_data, vWindOffset);
	#endif
	
	//position 					*= tree_position_and_scale_parameter.w;
	//position 					= adjust_for_tree_rotation(position, tree_rotation_parameter.xy);
	//position 					+= tree_position_and_scale_parameter.xyz;
	position 					= mul(m_W, float4(position, 1.0f));
	
	output.tc					= float2(input.tc.x, 1-input.tc.y);
	output.position				= position;//mul(m_V, float4(position, 1.f));
	
	float3 binormal 			= (cross(normal, tangent));
	
	output.normal				= mul( (float3x3)m_WV,	normal);
	
	vsf_tangent_space_transform	( normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.color				= normal_and_occl.w;
	output.color.a				= 1.001f;
	
	output.homogeneous_position = mul(m_VP, float4(position, 1.0f));
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
	parameters.normal 					= input.normal;
	parameters.color					= input.color;
	parameters.tangent_to_viewspace 	= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return 								parameters;
}

#endif // #ifndef SHADERS_SM_4_0_SPEEDTREE_FROND_VERTEX_INPUT_H_INCLUDED