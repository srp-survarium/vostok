#include "common.h"

uniform sampler2D       s_tonemap;              // actually MidleGray / exp(Lw + eps)


struct 	_in        	
{
	float4 	p		: POSITIONT;	// xy=pos, zw=tc0
	float3 	eye_ray : POSITION;		// quad corner eye rays 
	float2	tcJ		: TEXCOORD;	// jitter coords
};

struct  _out      
{
        float4     	hpos        : SV_POSITION;
		float3		eye_ray		: TEXCOORD0;
		
#ifdef        USE_VTF
        float4		tc0         : TEXCOORD1;    // tc.xy, tc.w = tonemap scale
#else
        float2		tc0         : TEXCOORD1;    // tc.xy
#endif

		float2		tcJ			: TEXCOORD2;	// jitter coords
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
_out 	main	( _in   I )
{
	_out 		O;
	O.hpos 		= float4	(I.p.x, -I.p.y, 0, 1);
#ifdef	USE_VTF
	float  	scale 	= tex2Dlod 	(s_tonemap,float4(.5,.5,.5,.5)).x ;
	O.tc0			= float4	(I.p.zw, scale, scale);
//	O.tc0		= float4	(I.p.zw, 0.f, 0.f);
#else
	O.tc0		= I.p.zw;
#endif
	O.eye_ray	= I.eye_ray;
	O.tcJ		= I.tcJ;

 	return	O;
}

FXVS;
