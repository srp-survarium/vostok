////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_POSTPROCESS_VERTEX_INPUT_H_INCLUDED
#define SHADERS_SM_4_0_POSTPROCESS_VERTEX_INPUT_H_INCLUDED

#include "common_functions.h"
#include "common_cbuffers.h"
#include "vsf_tangent_space_transform.h"

struct vertex_input_struct
{
	float3	position			: POSITION;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.position				= mul(m_WV,  float4(input.position, 1.f));
	output.homogeneous_position	= mul(m_WVP,  float4(input.position, 1.f));
}

material_parameters get_material_parameters(vertex_output_struct input)
{
	material_parameters 		parameters;
	clear_material_parameters	(parameters);
	return parameters;
}

#endif // #ifndef SHADERS_SM_4_0_POSTPROCESS_VERTEX_INPUT_H_INCLUDED