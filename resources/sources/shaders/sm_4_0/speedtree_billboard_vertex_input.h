////////////////////////////////////////////////////////////////////////////
//	Created		: 11.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPEEDTREE_BILLBOARD_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_SPEEDTREE_BILLBOARD_VERTEX_INPUT_H_INCLUDED

#include "common_defines.h"
#include "common_functions.h"
#include "common_cbuffers.h"
#include "speedtree_common.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float4	position_and_scale		: POSITION;
	float4	direction_and_rotation	: TEXCOORD0;
	uint	corner_index			: TEXCOORD1;
};

struct vertex_output_struct
{
	float4  homogeneous_position	: SV_Position;
	float3	position				: TEXCOORD0;
	float3	normal					: NORMAL;
	float2	tc						: TEXCOORD1;
	float3	tbn_x					: TEXCOORD2;
	float3	tbn_y					: TEXCOORD3;
	float3	tbn_z					: TEXCOORD4;
	float4	color					: TEXCOORD5;
};

// Parameters.

float4x4 	camera_facing_matrix_parameter;
float4		billboard_dimensions_parameter;
float4 		camera_azimuth_trig_parameter;
float4 		camera_angles_parameter;
float4 		billboard_tangents_parameter[3];
float4 		tex_coords_360_parameter[50];
float 		num360images_parameter;

float3		view_location_parameter;

float3 rotate_corner(float3 corner)
{
	return float3(dot(camera_azimuth_trig_parameter.ywz, corner.xyz), corner.y, dot(camera_azimuth_trig_parameter.xwy, corner.xyz));
}

float2 determine_billboard_tex_coords(float rotation, int corner_index) // rotation is in radians
{
	static const float2 coordinate_corners[4] =				
	{ 
		float2(1.0f, 0.0f), 
		float2(0.0f, 0.0f),
		float2(0.0f, 1.0f), 
		float2(1.0f, 1.0f),
	};
	
	// values derived from global shader constants
    const float  camera_azimuth 		= camera_angles_parameter.x;
	const float	 num_360_images 		= num360images_parameter;
    const float  angle_sweep_per_image 	= speedtree_pi_x2 / float(num_360_images);
	const float  half_sweep 			= 0.5f * angle_sweep_per_image;
	
	// get the tree angle into [0, 2pi] range
	float tree_angle 					= camera_azimuth + rotation + half_sweep + speedtree_pi;
	
	//#if (defined(SPEEDTREE_RHCS) && defined(SPEEDTREE_Y_UP))
	//	tree_angle 						-= speedtree_pi * 0.5;
	//#endif
	//#if (defined(SPEEDTREE_LHCS) && defined(SPEEDTREE_Z_UP))
	//	tree_angle 						= speedtree_pi - tree_angle + half_sweep;
	//#endif
	//#if (defined(SPEEDTREE_LHCS) && defined(SPEEDTREE_Y_UP))
		tree_angle 						= speedtree_pi_x2 - tree_angle + 2.0f * half_sweep;
	//#endif
	
	tree_angle 							= fmod(tree_angle, speedtree_pi_x2);
	
	if (tree_angle < 0.0f)
		tree_angle 						+= speedtree_pi_x2;
	
	// convert from tree angle to texcoord index
	int image_index 					= trunc(tree_angle / angle_sweep_per_image);
	
	// compute texcoords
	float4 tex_coords 					= tex_coords_360_parameter[image_index];
	
	return tex_coords.xy - tex_coords.zw * coordinate_corners[corner_index];
}

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	const float lod_near 				= lod_profile_parameter.z;
	const float	lod_range 				= lod_profile_parameter.w - lod_profile_parameter.z;
	
	float3 		position 				= input.position_and_scale.xyz;
	const float uniform_size			= input.position_and_scale.w;
	
	int 		index 					= input.corner_index;
	
	output.tc.xy						= determine_billboard_tex_coords(input.direction_and_rotation.w, index);
	output.tc.y							= 1.0f - output.tc.y;
	
	float dist							= distance(lod_reference_position_parameter.xyz, position);
	float visibility_scale 				= 1;//dist < lod_near ? 0.0 :uniform_size;
	
	float2 corner_coords;
	
	FLATTEN switch(index)
	{
		case 0:
			corner_coords 	= billboard_dimensions_parameter.xz;
			break;
		case 1:
			corner_coords 	= billboard_dimensions_parameter.yz;
			break;
		case 2:
			corner_coords 	= billboard_dimensions_parameter.yw;
			break;
		default:
			corner_coords 	= billboard_dimensions_parameter.xw;
			break;
	}
	
	//float billboard_scale_x 			= saturate(pow(dist/lod_range,2.0f));
	//corner_coords.x 					*= billboard_scale_x;
	
	float3 card_corner 					= (0.0f).xxx;
	
	// TODO: optimize it, calc in c++
	const float3 view_direction 		= normalize(view_location_parameter.xyz - position);
	const float3 tree_direction			= input.direction_and_rotation.xyz;
	const float3 right_offset 			= normalize(cross(view_direction, -tree_direction));
	const float3 up_offset 				= tree_direction;
	card_corner							= corner_coords.y * uniform_size * up_offset + -corner_coords.x * uniform_size * right_offset;
	
	#ifdef SPEEDTREE_BASIC_WIND
		card_corner 					= billboard_motion(card_corner / uniform_size, position.xz) * uniform_size;
	#endif
	
	position 							+= visibility_scale * card_corner;
	
	output.position						= position;
	
	output.normal						= mul( (float3x3)m_WV,	billboard_tangents_parameter[0].xyz);
	
	// setup tangents
	float3 normal 						= billboard_tangents_parameter[0].xyz;
	float3 binormal 					= -billboard_tangents_parameter[1].xyz;
	float3 tangent 						= billboard_tangents_parameter[2].xyz;
	
	vsf_tangent_space_transform			( normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z); 
	
	output.color						= 1.0f;//float4(1,0,0,0);//saturate(dist/lod_range);
	output.color.a						= compute_billboard_fade(dist) + 0.01f;
	
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

#endif // #ifndef SHADERS_SM_4_0_SPEEDTREE_BILLBOARD_VERTEX_INPUT_H_INCLUDED