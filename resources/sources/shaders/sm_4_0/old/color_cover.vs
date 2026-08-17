#include "common.h"

v2p_TL0uv_pos main ( v_TL0uv I) 
{
	v2p_TL0uv_pos vout;
	//vout.HPos = mul( m_WVP, float4(I.P,1));
	vout.HPos = mul( m_VP, float4(I.P,1));
	vout.Color = I.Color.bgra;
	vout.Pos = I.P;
	
 	return vout;
}