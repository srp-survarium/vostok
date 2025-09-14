////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPEEDTREE_LEAFCARD_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_SPEEDTREE_LEAFCARD_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "speedtree_common.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float4	card_center			: POSITION;	 // xyz = center pos, w = wind scalar
	uint4	normal				: TEXCOORD0; // xyz = normal, w = amb occ
	uint4	tangent				: TEXCOORD1; // xyz = tangent, w = tangent padding
	uint4	wind_data			: TEXCOORD2; // xyzw = wind data
	float4  card_corner			: TEXCOORD3; // xy = card corner, z = lod scale, w = wind scalar mag
	float4  tc					: TEXCOORD4; // xy = diffuse texcoords, z = planar offset, w = shadow offset
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

float4x4 camera_facing_matrix_parameter;

float2 get_max_scale(float3x3 in_matrix)
{
	const float3 scale = float3( length(in_matrix[0].xyz), length(in_matrix[1].xyz), length(in_matrix[2].xyz) );
	return max(scale.x, max(scale.y, scale.z)).xx;
}

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	const float  lod 			= tree_rotation_parameter.z;
	const float2 corner 		= input.card_corner.xy;
	const float	 lod_value 		= input.card_corner.z;
	const float  wind_scalar 	= input.card_center.w;
	const float  wind_scalar_mag= input.card_corner.w;
	const float4 wind_data 		= uncompress_wind_vector(input.wind_data, wind_scalar_mag);
	const float  planar_offset 	= input.tc.z;	
	
	float3 rotated_center 		= adjust_for_tree_rotation(input.card_center.xyz, tree_rotation_parameter.xy);
	
	rotated_center 				= mul(m_W, float4(input.card_center.xyz, 1.0f));
	
	float4 normal_and_occlusion = uncompress_vector4(input.normal);
	float3 normal 				= normal_and_occlusion.xyz;
	float3 tangent 				= uncompress_vector4(input.tangent).xyz;
		   //normal 				= adjust_for_tree_rotation(normal, tree_rotation_parameter.xy);
		   //tangent 				= adjust_for_tree_rotation(tangent, tree_rotation_parameter.xy);
	float3 binormal 			= cross(normal, tangent);
	
	float lod_scale 			= lerp(lod_value, 1.0f, lod);
	float2 lod_scaled_corner 	= get_max_scale((float3x3)m_W) * corner * lod_scale;
	float4 card_corner 			= float4(0.0f, lod_scaled_corner, 1.0f);
	card_corner 				= mul(camera_facing_matrix_parameter, card_corner);
	
	float3 position 			= rotated_center + card_corner.xyz;
	
	float dist					= distance(lod_reference_position_parameter.xyz, position);
	
	// adjust corner to keep all the leaf cards from being coplanar
	position 					+= planar_offset * eye_direction;
	
	// move the vertex with wind
	#ifdef SPEEDTREE_BASIC_WIND
		#ifdef SPEEDTREE_ADVANCED_WIND
			position 			= leaf_wind_tumble(
				position, 
				normal, 
				tangent, 
				binormal, 
				wind_scalar, 
				rotated_center
				);
		#else
			position 			= leaf_wind_motion(position, normal, wind_scalar);
		#endif
		float3 wind_offset;
		position 				= common_wind_motion(position, wind_data, wind_offset);
	#endif
	
	//position 					*= tree_position_and_scale_parameter.w;
	//position 					+= tree_position_and_scale_parameter.xyz;
	//position 					= mul(m_W, float4(position, 1.0f));
	
	output.tc					= float2(input.tc.x, 1-input.tc.y);
	output.position				= position;//mul(m_V, float4(position, 1.f));
	output.normal				= mul((float3x3)m_V, normal);
	
	vsf_tangent_space_transform	(normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.color				= normal_and_occlusion.w;
	output.color.a				= compute_3d_fade() + 0.01f;
//	output.color.rgb			= length(wind_data.xyz)*1.0f;
//	output.color.rgb			= normalize(wind_data.xyz);
//	output.color.a				= 1.1f;//compute_3d_fade() + 0.01f;
	
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

#endif // #ifndef SHADERS_SM_4_0_SPEEDTREE_LEAFCARD_VERTEX_INPUT_H_INCLUDED