////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPEEDTREE_BRANCH_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_SPEEDTREE_BRANCH_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "speedtree_common.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float3	position			: POSITION;  	// xyz = position
	float4	lod_pos_wind		: TEXCOORD0; 	// xyz = LOD position, w = wind scalar mag
	float4	tc					: TEXCOORD1; 	// xy = diffuse, zw = detail
	uint4	normal				: TEXCOORD2; 	// xyz = normal, w = amb occ
	uint4	tangent				: TEXCOORD3; 	// xyz = tangent, w = padding
	uint4	wind_data			: TEXCOORD4; 	// xyz = wind direction, w = wind offset
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	normal				: NORMAL;
	float4	tc					: TEXCOORD1;
	float3	tbn_x				: TEXCOORD2;
	float3	tbn_y				: TEXCOORD3;
	float3	tbn_z				: TEXCOORD4;
	float4	color				: TEXCOORD5;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	static const float lod		= tree_rotation_parameter.z;
	float3 position 			= lerp(input.lod_pos_wind.xyz, input.position.xyz, lod);
	float4 wind_data	  		= uncompress_wind_vector(input.wind_data, input.lod_pos_wind.w);	
	
	//float3 up					= tree_position_and_scale_parameter.xyz + float3(0, position.y, 0);
	//wind_data.xyz  			= 0.1f * distance(tree_position_and_scale_parameter.xyz + position, up);
	
	// move the branch vertex with wind
#ifdef SPEEDTREE_BASIC_WIND
	float3 wind_offset;
	position 					= common_wind_motion(position, wind_data, wind_offset);
#endif // #ifdef SPEEDTREE_BASIC_WIND
	
	//position 					*= tree_position_and_scale_parameter.w;
	//position 					= adjust_for_tree_rotation(position, tree_rotation_parameter.xy);
	position 					= mul(m_W, float4(position, 1.0f));
	//position 					+= tree_position_and_scale_parameter.xyz;
	
	output.tc.xy				= float2(input.tc.x, 1-input.tc.y);
	output.tc.zw				= input.tc.zw;
	
	float dist					= distance(eye_position, position);
	
	output.position				= position;
	
	float4 normal_and_occlusion = uncompress_vector4(input.normal);
	float3 normal 				= normal_and_occlusion.xyz;
	float3 tangent 				= uncompress_vector4(input.tangent).xyz;
		   //normal 				= adjust_for_tree_rotation(normal, tree_rotation_parameter.xy);
		   //tangent 				= adjust_for_tree_rotation(tangent, tree_rotation_parameter.xy);
	
	output.normal				= mul((float3x3)m_V, normal);
	float3 binormal 			= cross(normal, tangent);
	
	vsf_tangent_space_transform (normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.color				= normal_and_occlusion.w;
	//output.color				= length(wind_data.xyz)*1.0f;
	//output.color.rgb			= normalize(wind_data.xyz);
	//output.color				= wind_data.w;
	output.color.a				= compute_3d_fade() + 0.01f;
	
	output.homogeneous_position = mul(m_VP, float4(position, 1.0f));
}

material_parameters get_material_parameters(inout vertex_output_struct input)
{
	material_parameters 				parameters;
	clear_material_parameters			(parameters);
	
	parameters.world_position			= input.position;
	parameters.world_view_position 		= float4(mul(m_V, float4( input.position, 1.f)), 1.0f);
	parameters.world_view_proj_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	parameters.tc 						= input.tc.xy;
	parameters.detail_tc 				= input.tc.zw;
	parameters.normal 					= input.normal;
	parameters.color					= input.color;
	parameters.tangent_to_viewspace 	= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return 								parameters;
}

#endif // #ifndef SHADERS_SM_4_0_SPEEDTREE_BRANCH_VERTEX_INPUT_H_INCLUDED