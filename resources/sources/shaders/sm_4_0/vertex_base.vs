////////////////////////////////////////////////////////////////////////////
//	Created		: 01.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

/*	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
		CONFIG_WIND_MOTION,
*/

#include "common.h"
#include "material.h"
#include "vertex_input.h"

void main (in vertex_input_struct input,
		   out vertex_output_struct output)
{
	fill_output_vertex(input, output);
}










