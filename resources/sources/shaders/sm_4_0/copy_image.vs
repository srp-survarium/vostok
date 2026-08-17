////////////////////////////////////////////////////////////////////////////
//	Created		: 06.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "common.h"


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