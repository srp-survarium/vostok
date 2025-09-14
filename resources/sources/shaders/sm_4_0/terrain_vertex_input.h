////////////////////////////////////////////////////////////////////////////
//	Created		: 25.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_TERRAIN_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_TERRAIN_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

Texture2D						t_height;
uniform float4					terrain_size;
uniform float4					start_corner;

#define 	max_height	 		256.f


struct vertex_input_struct
{
	float2 position 			: POSITION;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3  position 			: TEXCOORD0;
	float3 	normal				: NORMAL;
	float2 	tc 					: TEXCOORD1;
 	float3	tbn_x				: TEXCOORD2;
 	float3	tbn_y				: TEXCOORD3;
 	float3	tbn_z				: TEXCOORD4;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output						= (vertex_output_struct)0;
	
	output.tc 					= float2( input.position.x /*- start_corner.x*/, -input.position.y/*-(input.position.y + start_corner.z)*/);
	float height 				= t_height.Load( int3( output.tc.xy, 0) ).x;
	
	output.position 			= float3( input.position.x+start_corner.x, height*max_height - max_height/2.f, input.position.y+start_corner.z);
	
	float3 position				= output.position;
	
	output.position				= float3 ( mul(m_WV, 	float4(output.position,1)).xyz);
	
	float h_0, h_1, h_2, h_3;
	int texture_width, texture_height;
	t_height.GetDimensions		( texture_width, texture_height);
	
	h_0 						= t_height.Load( int3( output.tc, 0) + int3( 0,(output.tc.y>0?-1:0), 0)).x;
	h_1 						= t_height.Load( int3( output.tc, 0) + int3((output.tc.x>0?-1:0), 0, 0)).x;
	h_2 						= t_height.Load( int3( output.tc, 0) + int3( (output.tc.x<texture_width-1?1:0), 0, 0)).x;
	h_3 						= t_height.Load( int3( output.tc, 0) + int3( 0, (output.tc.y<texture_height-1?1:0), 0)).x;
	
	h_0 						= h_0*max_height;
	h_1 						= h_1*max_height;
	h_2 						= h_2*max_height;
	h_3 						= h_3*max_height;
	
	float3 n;
	float texelAspect 			= 0.999f;
	n.z 						= -(h_0 - h_3) * texelAspect * ( output.tc.y<=0 || output.tc.y>=texture_height - 1 ? 2:1);
	n.x 						= (h_1 - h_2) * texelAspect * ( output.tc.x<=0 || output.tc.x>=texture_width - 1 ? 2:1);
	n.y 						= 2.f;
	
	float3 normal 				= normalize( n );
	float3 binormal 			= normalize( cross( normal, float3(1,0,0) ));
	float3 tangent 				= cross( binormal, normal );
	
	vsf_tangent_space_transform ( normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z);
	
	output.normal				= output.tbn_z;
	
	output.homogeneous_position = mul(m_WVP, float4(position, 1.0f));
}

material_parameters get_material_parameters(inout vertex_output_struct input)
{
	material_parameters 				parameters;
	clear_material_parameters			(parameters);
	
	parameters.world_view_position 		= float4(input.position, 1.0f);
	parameters.world_view_proj_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	parameters.tc 						= input.tc;
	parameters.detail_tc 				= input.tc;
	parameters.normal 					= normalize(input.tbn_z); // ?
	parameters.tangent_to_viewspace 	= float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	
	return 								parameters;
}

#endif // #ifndef SHADERS_SM_4_0_TERRAIN_VERTEX_INPUT_H_INCLUDED