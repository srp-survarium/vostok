#include "common.h"

struct v_sky_box
{
	float4 p        : POSITION;
	float4 c        : COLOR0;
	float3 tc0      : TEXCOORD0;
	float3 tc1      : TEXCOORD1;
};

struct v2p_sky_box
{
	float4 factor	: COLOR0;
	float3 tc		: TEXCOORD0;
	float4 hpos		: SV_POSITION;
};

void main ( in v_sky_box input,
		    out v2p_sky_box output)
{
	output.hpos     = mul(m_WVP, input.p);
	output.hpos.z	= output.hpos.w;
	output.tc      	= input.p.xyz;
	output.factor   = input.c;
}
