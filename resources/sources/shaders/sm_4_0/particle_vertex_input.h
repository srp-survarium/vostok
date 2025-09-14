////////////////////////////////////////////////////////////////////////////
//	Created		: 05.08.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_PARTICLE_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_PARTICLE_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "common_defines.h"

#include "vsf_tangent_space_transform.h"

// TODO: Pack this values!

float3   up_view_vector;
float3   right_view_vector;

float	 use_align_by_dir;
float3   view_location;

float	 use_fixed_axis;
float3   rotation_fixed_axis;


/*
enum enum_particle_locked_axis {
	particle_locked_axis_x,
	particle_locked_axis_y,
	particle_locked_axis_z,
	particle_locked_axis_negative_x,
	particle_locked_axis_negative_y,
	particle_locked_axis_negative_z,
	- particle_locked_axis_rotate_x,
	- particle_locked_axis_rotate_y,
	- particle_locked_axis_rotate_z,
	- particle_locked_axis_none,
}; // enum enum_particle_locked_axis
*/

float locked_no_ratate_axis_index;
// up, right
static const float3 locked_axis[6][2] = 
{
	{float3(0,1,0), float3(0,0,-1)},
	{float3(0,0,1), float3(1,0,0)},
	{float3(0,1,0), float3(-1,0,0)},
	
	{float3(0,1,0), float3(0,0,1)},
	{float3(0,0,-1), float3(1,0,0)},
	{float3(0,1,0), float3(1,0,0)},
};

struct vertex_input_struct
{
	float3	position			: POSITION;
	float4	color				: TEXCOORD0;
	float2	tc					: TEXCOORD1;  // Optimize uv_index, add index.
	float2	size				: TEXCOORD2;
	float	rotation			: TEXCOORD3;
	float3	prev_position		: TEXCOORD4;
#if CONFIG_USE_SUBUV
	float2	size_tc				: TEXCOORD5;
	float4	blend_tc			: TEXCOORD6;
#endif // #if CONFIG_USE_SUBUV
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	normal				: NORMAL;
	float4	color				: COLOR;
	float2	tc					: TEXCOORD1;
	float3  tangent				: TEXCOORD2;
	float3  binormal			: TEXCOORD3;
#if CONFIG_USE_SUBUV	
	float4	tc1					: TEXCOORD4;
#endif // #if CONFIG_USE_SUBUV
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output 						= (vertex_output_struct)0;
	
	// m_W identity if not using local space transform.
	input.position  			= mul( m_W, float4(input.position.xyz, 1.0f) );
	input.prev_position 		= mul( m_W, float4(input.prev_position.xyz, 1.0f) );
	
	float3 position 			= input.position;
		
	float sine_rotation, cosine_rotation;
	sincos(input.rotation, sine_rotation, cosine_rotation);
	
	/*
		c -s
		s  c
	*/
	
	float3 right_rotated		= cosine_rotation * right_view_vector.xyz - sine_rotation * up_view_vector.xyz;
	float3 up_rotated			= sine_rotation * right_view_vector.xyz   + cosine_rotation * up_view_vector.xyz;
	
	float3 view_direction  	 	= normalize(view_location.xyz - position);
	
	// TODO: fix prev position!!!!!
	float3 particle_direction 	= normalize(position - input.prev_position);
    
	float3 right_aligned		= -normalize(cross(view_direction, particle_direction));
	
	float3 up_result    		= up_rotated;
	float3 right_result 		= right_rotated;
	
	// TODO: Dont use if
	//BRANCH
	if (use_align_by_dir>0.0f)
	{
		right_result 			= normalize(cross(view_direction, particle_direction));
		up_result 				= particle_direction;
	}
	
	// TODO: Dont use if
	//BRANCH
	if (use_fixed_axis>0.0f)
	{
		right_result			= normalize(cross(view_direction, rotation_fixed_axis.xyz));
		up_result				= rotation_fixed_axis.xyz;
	}
	
	//BRANCH
	if (locked_no_ratate_axis_index>=0.0f)
	{
		int index				= floor(locked_no_ratate_axis_index);
		right_result			= locked_axis[index][1];
		up_result				= locked_axis[index][0];
	}
	
#if CONFIG_USE_SUBUV
	position    				+= (-input.size_tc.y + 0.5) * input.size.y * up_result     +
							    (input.size_tc.x - 0.5) * input.size.x * right_result;
	output.tc1		  			= input.blend_tc;
#else
	position    				+= (-input.tc.y + 0.5) * input.size.y * up_result     +
							    (input.tc.x - 0.5) * input.size.x * right_result;
#endif // #if CONFIG_USE_SUBUV
	
	output.color	  			= input.color;
	output.tc		  			= input.tc;
	
	output.position				= position;//mul(m_V,  float4(position.xyz,1)).xyz;
	
	// TODO: inverted binormal?
	output.tangent				= normalize(right_result);
	output.binormal				= normalize(up_result);
	output.normal				= cross(up_result, right_result);
	
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
#if CONFIG_USE_SUBUV
	parameters.sub_uv					= input.tc1;
#endif // #if CONFIG_USE_SUBUV
	parameters.detail_tc 				= input.tc;
	parameters.color 					= input.color;
	parameters.normal 					= mul(m_V, float4(input.normal, 0.0f)).xyz;
	parameters.tangent_to_viewspace 	= mul((float3x3)m_V, transpose(float3x3(input.tangent, input.binormal, input.normal)));
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_PARTICLE_VERTEX_INPUT_H_INCLUDED
