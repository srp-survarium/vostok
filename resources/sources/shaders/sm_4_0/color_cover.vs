////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-18 from the shipped v0.100b blob; the pre-ship
//	revision is in old/. Byte-identity: python3 -m vostok.shaders roundtrip
////////////////////////////////////////////////////////////////////////////

#include "common.h"

v2p_TL0uv_pos main ( v_TL0uv I) 
{
	v2p_TL0uv_pos vout;

	vout.HPos = mul( m_VP, float4(I.P,1));
	vout.Color = I.Color;
	vout.Pos = I.P;

 	return vout;
}
