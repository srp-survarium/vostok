#include "common.h"

struct simple_vert
{
	float3 P : POSITION;
	float2 tc : TEXCOORD0;
};

v2p_flat 	main	( simple_vert I )
{
	// Eye-space pos/normal
	v2p_flat 		O;
	float4	Pp 	= mul		(m_WVP,	float4(I.P, 1.f));
	O.hpos 		= Pp;
	O.N 		= mul		((float3x3)m_WV,	unpack_bx2( float3( 0.f, 1.f, 0.f)));
	float3	Pe	= mul		(m_WV,  I.P		);

	//float2	tc 	= unpack_tc_base	(I.tc,I.T.w,I.B.w);	// copy tc
	//float2	tc 	= I.tc*16.f/32768.f;//
	O.tcdh		= I.tc.xy;
	O.position	= float4	(Pe, 	0);

#if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI)
	float 	s	= I.color.w	;							// (r,g,b,dir-occlusion)
	O.tcdh.w	= s;
#endif

#ifdef	USE_TDETAIL
	O.tcdbump	= O.tcdh * dt_params;					// dt tc
#endif

#ifdef	USE_LM_HEMI
	O.lmh 		= unpack_tc_lmap	(I.lmh);
#endif

	return	O;
}
FXVS;
