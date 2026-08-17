////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-18 from the shipped v0.100b blob; the pre-ship
//	revision is in old/. Byte-identity: python3 -m vostok.shaders roundtrip
////////////////////////////////////////////////////////////////////////////

#include "common.h"

v2p_TL0uv main ( v_TL0uv I) 
{
	v2p_TL0uv vout;

	vout.HPos = mul( m_WVP, float4(I.P,1));
	vout.HPos.z = 0;
	vout.Color = I.Color;

 	return vout;
}
