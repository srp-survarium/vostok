////////////////////////////////////////////////////////////////////////////
//	Created		: 05.08.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

/*	
	$DEFINES$: 
		GLOBAL_USE_BRANCHING,
		CONFIG_USE_SUBUV,
*/

#define V2P_PARAM_H_POSITION 	1
#define V2P_PARAM_POSITION 		1
#define V2P_PARAM_TC0		 	1
#define V2P_PARAM_TC1		 	1
#define V2P_PARAM_COLOR0	 	1

#include "common_defines.h"
#include "v_particle_sprite.h"
#include "v2p_common.h"
#include "v2p_base.h"

uniform float4x4 m_VP;
uniform float3x4 m_W;


// TODO: Pack this values!

float3   up_view_vector;
float3   right_view_vector;

float	 use_align_by_dir;
float3   view_location;

float	 use_fixed_axis;
float3   rotation_fixed_axis;

uniform float3x4	m_V;
uniform float3x4	m_WV;

/*
	TODO: remove CONFIG_USE_SUBUV, add vs constant - emitter subimage index, not per particle additional UVs ?
*/

struct vertex_output_struct
{
	float3	position		: TEXCOORD0;
	float3	normal			: NORMAL;
	float4	color			: COLOR;
	float2	tc				: TEXCOORD1;
	float3  tangent			: TEXCOORD2;
	float3  binormal		: TEXCOORD3;
#if CONFIG_USE_SUBUV	
	float4	tc1				: TEXCOORD4;
#endif // #if CONFIG_USE_SUBUV
};

void main ( in v_particle_sprite input,
			out vertex_output_struct output,
			out float4 hposition : SV_Position
	)
{
	output						= (vertex_output_struct)0;
	
	// m_W identity if not using local space transform.
	input.position  			= mul( m_W, float4(input.position.xyz, 1.0f) );
	input.prev_position 		= mul( m_W, float4(input.prev_position.xyz, 1.0f) );
	
	float3 position 			= input.position;
	
	float rotation_angle		= input.rotation;
	
	float3 up_rotated			= sin(rotation_angle) * up_view_vector.xyz 	  + cos(rotation_angle) * right_view_vector.xyz;
	float3 right_rotated		= sin(rotation_angle) * right_view_vector.xyz - cos(rotation_angle) * up_view_vector.xyz;
	
	float3 view_direction  	 	= normalize(position - view_location.xyz);
	float3 particle_direction 	= normalize(position - input.prev_position);
    
	float3 up_aligned			= -particle_direction;	
	float3 right_aligned		= normalize(cross(view_direction, particle_direction));
	
	float3 up_result    		= up_rotated;
	float3 right_result 		= right_rotated;
	
	// TODO: Dont use if
	BRANCH
	if (use_align_by_dir>0.0f)
	{
		up_result 				= up_aligned;
		right_result 			= right_aligned;
	}
	
	// TODO: Dont use if
	BRANCH
	if (use_fixed_axis>0.0f)
	{
		up_result				= normalize(cross(view_direction, rotation_fixed_axis.xyz));
		right_result			= rotation_fixed_axis.xyz;
	}
	
#if CONFIG_USE_SUBUV
	position    				+= (input.size_tc.y - 0.5) * input.size.y * up_result     +
							    (input.size_tc.x - 0.5) * input.size.x * right_result;
	output.tc1		  			= input.blend_tc;
#else
	position    				+= (input.tc.y - 0.5) * input.size.y * up_result     +
							    (input.tc.x - 0.5) * input.size.x * right_result;
#endif // #if CONFIG_USE_SUBUV
	
	output.color	  			= input.color;
	output.tc		  			= input.tc;
	//output.tc.x = floor(output.tc.x * 4.0f) / 4.0f;
	
	hposition  					= mul(m_VP, float4(position,1));
	output.position				= mul(m_V,  float4(position.xyz,1)).xyz;
}

























