////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_GRASSMESH_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_GRASSMESH_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float3	position					: POSITION;
	float3	normal						: NORMAL;
	float3	tangent						: TANGENT;
	float3	binormal					: BINORMAL;
	float2	tc							: TEXCOORD0;
	float4	world_transform_0			: WorldTransform0;
	float4	world_transform_1			: WorldTransform1;
	float4	world_transform_2			: WorldTransform2;
	float4	grass_color					: GrassColor;
};

struct vertex_output_struct
{
	float4  homogeneous_position		: SV_Position;
	float3	position					: TEXCOORD0;
	float3	normal						: NORMAL; 	// TODO: Remove normal.
	float2	tc							: TEXCOORD1;
	float3	tbn_x						: TEXCOORD2;
	float3	tbn_y						: TEXCOORD3;
	float3	tbn_z						: TEXCOORD4;
	float4	color						: TEXCOORD5;
};

void add_base_wind_motion_local(inout float3 position, in float3 local_position, in float4x4 wm)
{
	float dist  						= local_position.y * length(local_position);
	float s, c;
	
	float3 object_pos					=	float3(wm[0][3], wm[1][3], wm[2][3]);
	
	const float period					=	0.5f;
	const float movement_mult 			=	0.2f;
	
	s									=	sin(5 * scene_time + period * object_pos.x);
	c									=	cos(5 * scene_time + period * object_pos.z);
	
	position.x 							+=  movement_mult * dist * s;
	position.z 							+=  movement_mult * dist * c;
}

void add_base_wind_motion(inout float3 position, in float3 local_position, in float4x4 wm)
{
	float dist  						= local_position.y * length(local_position);
	float s, c;
	float s2, c2;
	
	float3 object_pos					=	float3(wm[0][3], wm[1][3], wm[2][3]);
	
	const float period					=	0.25f;
	const float movement_mult 			=	0.7125f;
	
	s									=	sin(2 * scene_time + period * object_pos.x);
	c									=	cos(2 * scene_time + period * object_pos.z);
	
	position.x 							+=  movement_mult * dist * s;
	position.z 							+=  movement_mult * dist * c;
}


// Optimize it

void tangent_space_transform(
		float4x4 wm, 
		float3 normal, 
		float3 tangent, 
		float3 bitangent, 
		out float3 tbn_x, 
		out float3 tbn_y, 
		out float3 tbn_z
	)
{
	float3x3 TBN 						= 	mul(
												(float3x3)mul(m_V, wm),
												transpose(
													float3x3(	
														tangent.xyz,
														bitangent,
														normal.xyz
													)
												)
											);
	
	tbn_x 	 							= 	TBN[0];
	tbn_y 	 							= 	TBN[1];
	tbn_z 	 							= 	TBN[2];
}

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 normal						=	unpack_bx4( unpack_D3DCOLOR( input.normal));
	float3 tangent						=	unpack_bx4( unpack_D3DCOLOR( input.tangent));
	float3 binormal						=	unpack_bx4( unpack_D3DCOLOR( input.binormal));
	
	//float3(0.0f, 1.0f, 0.0f);//
	//float3(1.0f, 0.0f, 0.0f);//
	//float3(0.0f, 0.0f, 1.0f);//
	
	float4x4 world_transform			=	0;
	world_transform[0]					=	float4(input.world_transform_0.xyz, 0.0f);
	world_transform[1]					=	float4(input.world_transform_1.xyz, 0.0f);
	world_transform[2]					=	float4(input.world_transform_2.xyz, 0.0f);
	world_transform[3]					=	float4(
												input.world_transform_0.w,
												input.world_transform_1.w,
												input.world_transform_2.w,
												1.0f
											);
	
	world_transform						=	transpose(world_transform);
	
	add_base_wind_motion_local			(input.position, input.position, world_transform);
	
	output.position						=	mul(world_transform,  float4(input.position, 1.f));
	output.normal						=	mul((float3x3)mul(m_V, world_transform), input.normal); // TODO: Remove normal.
	
	float3 object_pos					=	float3(world_transform[0][3], world_transform[1][3], world_transform[2][3]);
	float3 prev_pos						=	output.position;
	float3 to_vertex					=	output.position - object_pos;
	
	add_base_wind_motion				(output.position, input.position, world_transform);
	
	float3 to_vertex2					=	output.position - object_pos;
	
	output.position						+=	normalize(-to_vertex2) * max(length(to_vertex2) - length(to_vertex), 0.0f);
	
	output.color						=	float4(pow(input.position.y, 1.0f) * input.grass_color.xyz, 1.0f);
	
	output.tc 							=	input.tc;
	
	tangent_space_transform				(world_transform, normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.homogeneous_position			=	mul(m_VP, float4(output.position.xyz, 1.0f));
}

material_parameters get_material_parameters(inout vertex_output_struct input)
{
	material_parameters 				parameters;
	clear_material_parameters			(parameters);
	
	parameters.world_position			=	input.position;
	parameters.world_view_position 		=	float4(mul(m_V, float4( input.position, 1.f)), 1.0f);
	parameters.world_view_proj_position =	float4(0.0f, 0.0f, 0.0f, 1.0f);
	parameters.tc 						=	input.tc;
	parameters.detail_tc 				=	input.tc;
	parameters.normal 					=	normalize(input.normal); // TODO: Remove normal.
	parameters.tangent_to_viewspace 	=	float3x3( input.tbn_x, input.tbn_y, input.tbn_z);
	parameters.color					=	input.color;
	
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_GRASSMESH_VERTEX_INPUT_H_INCLUDED
