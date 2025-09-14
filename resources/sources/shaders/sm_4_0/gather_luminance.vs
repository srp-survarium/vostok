////////////////////////////////////////////////////////////////////////////
//	Created		: 28.04.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "common.h"

uniform float4 screen_res; // Screen resolution (x-Width,y-Height, zw - 1/resolution)

v2p_TL main ( v_TL_positiont I )
{
	v2p_TL O;
	
	O.HPos.x = I.P.x * screen_res.z * 2 - 1;
	O.HPos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
	O.HPos.zw = I.P.zw;
	
	O.Tex0 = I.Tex0;
	O.Color = I.Color;
	
 	return O;
}