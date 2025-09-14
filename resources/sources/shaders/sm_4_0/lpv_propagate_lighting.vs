////////////////////////////////////////////////////////////////////////////
//	Created		: 26.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_input_struct
{
	float4 position				: POSITION;
	float4 xy_and_slice_index	: TEXCOORD0;
};

struct vertex_output_struct
{
	float4 position				: POSITION;
	float4 xy_and_slice_index	: TEXCOORD0;
};

void main(in vertex_input_struct 	input,
		  out vertex_output_struct 	output)
{
	output.position 			=	input.position;
	output.xy_and_slice_index 	=	input.xy_and_slice_index;
}















