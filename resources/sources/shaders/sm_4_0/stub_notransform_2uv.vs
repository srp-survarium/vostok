#include "common_iostructs.h"

uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL2uv main ( v_TL2uv I )
{
	v2p_TL2uv	O;

	O.HPos.x = I.P.x * screen_res.z * 2 - 1;
	O.HPos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
	O.HPos.zw = I.P.zw;
	O.HPos.w = 1;

	O.Tex0 = I.Tex0;
	O.Tex1 = I.Tex1;
	//	Some shaders that use this stub don't need Color at all
	O.Color = I.Color.bgra;

 	return O;
}