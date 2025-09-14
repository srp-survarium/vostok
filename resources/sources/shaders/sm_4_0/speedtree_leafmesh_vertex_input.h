////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPEEDTREE_LEAFMESH_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_SPEEDTREE_LEAFMESH_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "speedtree_common.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float4	position			: POSITION;  // xyz = position, w = wind scalar
	float4	lod_pos_wind		: TEXCOORD0; // xyz = LOD position, w = wind scalar mag
	uint4	normal				: TEXCOORD1; // normal.xyz + amb occ
	uint4	tangent				: TEXCOORD2;
	uint4	wind_data			: TEXCOORD3; // xyz = wind direction, w = wind offset
	float2	tc					: TEXCOORD4; // xy = diffuse texcoords
#ifdef SPEEDTREE_ADVANCED_WIND
	float3	leafmesh_wind   	: TEXCOORD5; // xyz = leaf mesh specific wind data
#endif // #ifdef SPEEDTREE_ADVANCED_WIND
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
	float4 normal_and_occlusion = uncompress_vector4(input.normal);
	float3 normal 				= normal_and_occlusion.xyz;
	float4 tangent4 			= uncompress_vector4(input.tangent);
	float3 tangent 				= tangent4.xyz;
		   //normal 				= adjust_for_tree_rotation(normal, tree_rotation_parameter.xy);
		   //tangent 				= adjust_for_tree_rotation(tangent, tree_rotation_parameter.xy);
	float3 binormal 			= (cross(normal, tangent));
		
	const float lod 			= tree_rotation_parameter.z;
	const float wind_scalar 	= input.position.w;
	const float wind_active 	= (tangent4.w > 0.0f);
	const float wind_scalar_mag = input.lod_pos_wind.w;
	const float4 wind_data 		= uncompress_wind_vector(input.wind_data, wind_scalar_mag);
#ifdef SPEEDTREE_ADVANCED_WIND
	const float3 leaf_mesh_wind = input.leafmesh_wind.xyz;
#endif
	
	float3 position 			= lerp(input.lod_pos_wind.xyz, input.position.xyz, lod);
	
	// move the branch vertex with wind
	#ifdef SPEEDTREE_BASIC_WIND
		if (wind_active > 0.0f)
		{
			#ifdef SPEEDTREE_ADVANCED_WIND
				position 		= leaf_wind_tumble(
					position, 
					normal, 
					tangent, 
					binormal, 
					wind_scalar, 
					adjust_for_tree_rotation(leaf_mesh_wind, tree_rotation_parameter.xy)
					);
			#else
				position 		= leaf_wind_motion(position, normal, wind_scalar);
			#endif
			
			float3 wind_offset;
			position 			= common_wind_motion(position, wind_data, wind_offset);
		}
	#endif
	
	//position 					*= tree_position_and_scale_parameter.w;
	//position 					= adjust_for_tree_rotation(position, tree_rotation_parameter.xy);
	//position 					+= tree_position_and_scale_parameter.xyz;
	position 					= mul(m_W, float4(position, 1.0f));
	
	output.tc					= float2(input.tc.x, 1-input.tc.y);
	
	output.position				= position;//mul(m_V, float4(position, 1.f));
	
	output.normal				= mul( (float3x3)m_WV,	normal);
	
	vsf_tangent_space_transform	( normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.color				= normal_and_occlusion.w;
	output.color.a				= compute_3d_fade() + 0.01;
	
	//output.color.rgb			= length(wind_data.xyz)*1.0f;
	//output.color.rgb			= normalize(wind_data.xyz);
	//output.color.a			= 1.1f;//compute_3d_fade() + 0.01f;
	//output.color.rgb			= input.normal.xyz;
	
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
	parameters.tangent_to_viewspace 	= float3x3(input.tbn_x, input.tbn_y, input.tbn_z);
	
	return 								parameters;
}

#endif // #ifndef SHADERS_SM_4_0_SPEEDTREE_LEAFMESH_VERTEX_INPUT_H_INCLUDED