////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 from the shipped v0.100b blob - absent from the
//	recovered source drop. Byte-identity: python3 -m vostok.shaders roundtrip
////////////////////////////////////////////////////////////////////////////

#include "common.h"

struct v_in
{
	float3	position	: POSITION;
};

float4 main( v_in I, out float depth : TEXCOORD0 ) : SV_Position
{
	float4	position	= float4( I.position, 1 );

	depth				= mul( m_WV, position ).z;

	return	mul( m_WVP, position );
}
