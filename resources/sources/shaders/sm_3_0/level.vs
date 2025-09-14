#include "common.h"

p_flat 	main	( v_static I )
{
	// Eye-space pos/normal
	p_flat 	O;

	float4	Pp;
	float3	Pe;
	float2	tc;

 	Pp			= mul(m_WVP, I.P);
	Pe			= mul(m_WV, I.P);
	tc			= unpack_tc_base(I.tc, I.T.w, I.B.w);	// copy tc

	O.hpos 		= Pp;
	O.N 		= mul((float3x3)m_WV, unpack_bx2(I.Nh));
	O.tcdh		= float4(tc.xyyy);
	O.position	= float4(Pe, I.Nh.w);

	return	O;
}
FXVS;
