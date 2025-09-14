#include "common.h"

v2p_TL0uv main ( v_TL0uv I) 
{
	v2p_TL0uv vout;
	vout.HPos = mul( m_WVP, float4(I.P,1));
	vout.HPos.z = 0;
	vout.Color = I.Color.bgra;
	
 	return vout;
}