////////////////////////////////////////////////////////////////////////////
//	Created		: 06.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

/*
	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
*/

#include "material.h"
#include "vertex_input.h"

uniform float4 screen_res; // Screen resolution (x-Width,y-Height, zw - 1/resolution)

void main ( in vertex_input_struct input,
			out vertex_output_struct output)
{
	fill_output_vertex(input, output);
}
