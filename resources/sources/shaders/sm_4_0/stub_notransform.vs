#include "common_iostructs.h"

uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL main ( v_dumb In )
{
	v2p_TL O;

	O.HPos.x 	= In.P.x * screen_res.z * 2 - 1;
	O.HPos.y 	= (In.P.y * screen_res.w * 2 - 1)*-1;
	O.HPos.zw 	= In.P.zw;
	O.HPos.w	= 1;

 	return O;
}