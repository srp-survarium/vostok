struct test_vert
{
	float3 P 	: POSITION;
	float3 N 	: NORMAL;
	float2 TC0 	: TEXCOORD0;
};

struct test_vert_out
{
	float4 p 	: SV_POSITION;
	float4 c 	: COLOR;
	float2 tc 	: TEXCOORD0;
	float3 vsp 	: TEXCOORD1;
};                

uniform float4x4 	m_WVP;
uniform float4x4 	m_WV;

test_vert_out main(test_vert input)
{
	test_vert_out output;
	
	output.p 		= mul(m_WVP, float4(input.P,1));
	output.vsp		= mul(m_WV,  float4( input.P,1)).xyz;
	
	output.c.rgb    = input.N;
	output.c.a		= 1;
	output.tc		= input.TC0;
	return output;
}