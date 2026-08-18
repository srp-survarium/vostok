////////////////////////////////////////////////////////////////////////////
//	Created		: 28.04.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-18.  The shipped blob is a bare fullscreen
//	passthrough: its ISGN is POSITION/TEXCOORD only (no COLOR) and the
//	position is copied unmodified, so the screen_res half-pixel fixup the
//	recovered revision applied was gone by ship.  Same DXBC as the shipped
//	rain_resolve.vs / post_process_fxaa.vs passthrough.
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct vertex_input_struct
{
	float4 position	: POSITION;
	float2 uv		: TEXCOORD0;
};

struct vertex_output_struct
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

vertex_output_struct main ( vertex_input_struct input )
{
	vertex_output_struct 	output;

	output.position 		= input.position;
	output.uv				= input.uv;

 	return output;
}
