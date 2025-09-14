////////////////////////////////////////////////////////////////////////////
//	Created		: 16.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "common_cbuffers.h"
#include "common_functions.h"

struct vertex_input_struct
{
    float3 	position 		: POSITION;
	float3  color			: LightColor;
	float3  base_position	: LightPosition;
	float   range			: LightRange;
};

struct vertex_output_struct
{
    float4 	position 		: SV_Position;
	float3  color			: LightColor;
	float3  base_position	: LightPosition;
	float   range			: LightRange;
};

vertex_output_struct main(vertex_input_struct input)
{
	vertex_output_struct		output;
	
	output.position				= mul(m_WVP, float4(input.position * input.range + input.base_position, 1.0f));
	output.color				= input.color;
	output.base_position		= input.base_position;
	output.range				= input.range;
	
	return						output;
}