////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 from the shipped v0.100b blob - absent from the
//	recovered source drop. Byte-identity: python3 -m vostok.shaders roundtrip
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct v_in
{
	float4	position	: POSITION;
	float2	tc			: TEXCOORD0;
};

float4 main( v_in I, out float3 reflection : TEXCOORD0 ) : SV_Position
{
	float4	position	= float4( I.position.xyz, 1.0f);

	float3	world		= mul( m_W, position);
	float3	to_eye		= normalize( eye_position - world);
	float3	normal		= normalize( I.position.xyz);

	reflection			= -reflect( to_eye, normal);

	return	mul( m_WVP, position);
}
