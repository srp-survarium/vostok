////////////////////////////////////////////////////////////////////////////
//	Created		: 20.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

/*	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
*/

#include "material.h"
#include "vertex_input.h"

float4 gs_test_constant;

[maxvertexcount(3)]
void main(triangle vertex_output_struct input[3], inout TriangleStream<vertex_output_struct> output_triangle_stream)
{
	vertex_output_struct output[3];
	
	output[0] = input[0];
	output[1] = input[1];
	output[2] = input[2];
	
#if CONFIG_VERTEX_INPUT_TYPE==STATIC_VERTEX_INPUT_TYPE
	//float4 diffuse_color = t_base.SampleLevel(s_base, input[0].tc, 0);
	//output[0].tc *= gs_test_constant.x;
	//output[1].tc *= gs_test_constant.y;
	//output[2].tc *= gs_test_constant.z;
	//output[0].homogeneous_position.xy += diffuse_color.xy * 5;
	//output[1].homogeneous_position.xy += diffuse_color.xy * 5;
	//output[2].homogeneous_position.xy += diffuse_color.xy * 5;
#endif // #if CONFIG_VERTEX_INPUT_TYPE==STATIC_VERTEX_INPUT_TYPE
	
	output_triangle_stream.Append(output[0]);
	output_triangle_stream.Append(output[1]);
	output_triangle_stream.Append(output[2]);
}









