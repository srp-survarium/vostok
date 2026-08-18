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
