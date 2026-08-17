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

float4 main( v_in I, out float2 tc : TEXCOORD0 ) : SV_POSITION
{
	tc	= I.tc;

	return	mul( m_WVP, float4( I.position.xyz, 1.0f));
}
