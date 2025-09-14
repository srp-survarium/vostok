#include "terrain.h"

uniform half3x4	    m_WV;
uniform half4x4 	m_WVP;

terrain_vert_out main(terrain_vert input)
{
	terrain_vert_out output;
	
	output.hpos		= mul(m_WVP, float4(input.P,1));
	output.P 		= half4 ( mul(m_WV, 	float4(input.P,1)).xyz, 1.h /*hemi*/ );
	output.N		= mul(m_WV, float3(0.f,1.f,0.f));//input.N);

	output.c0.rgba  = input.c0;
	output.c1.rgba 	= input.c1;
	output.c2.rgba 	= input.c2;

	output.tc0		= input.TC0;
	output.tc1		= input.TC1;
	output.tc2		= input.TC2;	
	
	return output;
}