#include "common.h"

v2p_TL0uv main ( v_TL0uv I) 
{
	v2p_TL0uv vout;

	// Ship pulls the debug line 2cm toward the eye in view space, then
	// projects - not a single m_VP transform.
	float3 view_position	= mul( m_V, float4(I.P,1));
	view_position.z			-= 0.02f;

	vout.HPos	= mul( m_P, float4( view_position, 1));
	vout.Color	= I.Color;

 	return vout;
}
