////////////////////////////////////////////////////////////////////////////
//	Created		: 13.05.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

/*	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
*/

#include "material.h"
#include "vertex_input.h"

void main ( in vertex_input_struct input,
			out vertex_output_struct output)
{
	fill_output_vertex(input, output);
	
	output.homogeneous_position.xy = input.tc * 2.0f - 1.0f;
	output.homogeneous_position.y  *= -1.0f;
	output.homogeneous_position.z  = 0.0f;
	output.homogeneous_position.w  = 1.0f;
	
	//float3x3 view_space_to_tangent_space = float3x3(output.tbn_x, output.tbn_y, output.tbn_z);
	//float3 tangent_normal = mul(view_space_to_tangent_space, output.normal);
	//position.xy += tangent_normal.xy * 0.005f;
}