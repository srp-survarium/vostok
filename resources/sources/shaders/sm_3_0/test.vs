struct test_vert
{
	float3 P 	: POSITION;
	float3 N 	: NORMAL;
	float2 TC0 	: TEXCOORD0;
};

struct test_vert_out
{
	half4 p 	: POSITION;
	half4 c 	: COLOR;
	half2 tc 	: TEXCOORD0;
};                

uniform float4x4 	m_WVP;

test_vert_out main(test_vert input)
{
	test_vert_out output;
	
	output.p 		= mul(m_WVP, float4(input.P,1));
//	output.p 		= mul(m_VP, input.P);

	output.c.rgb    = input.N;
	output.c.a		= 1;
	output.tc		= input.TC0;
	return output;
}