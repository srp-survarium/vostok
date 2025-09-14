////////////////////////////////////////////////////////////////////////////
//	Created		: 05.04.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

/*	$DEFINES$:
*/

struct vertex_input_struct
{
	float4 position	: POSITION;
	float2 tc		: TEXCOORD0;
};

struct vertex_output_struct
{
	float2 tc		: TEXCOORD0;
};

void main (in  vertex_input_struct  input,
		   out vertex_output_struct output,
		   out float4 out_position	: SV_POSITION)
{
	out_position = float4(input.position.xy, 0.0f, 1.0f);
	
	int face_index = (int)input.position.z;
	
	output.tc 	 = input.tc;
/*	
	output.tc.y	/= 2.0f;
	output.tc.x	/= 3.0f;
	
	output.tc.y += (face_index % 2) * 0.5f;
	output.tc.x += (face_index / 2) * 0.33333f;*/
}
