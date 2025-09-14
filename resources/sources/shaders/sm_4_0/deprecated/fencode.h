#ifndef	FENCODE_H
#define FENCODE_H

#include "common.h"

//////////////////////////////////////////////////////////////////////////////////////////
uniform float3		v_encodeZ01;
uniform float3		v_decodeZ01;
float3	encode_tcRG	( float  z )	{ return z*v_encodeZ01.xyz;	}
float2	encode_tcB	( float  z )	{ return z*v_encodeZ01.z;	}
//////////////////////////////////////////////////////////////////////////////////////////
// 0..1 encoding with 21 bit precision
static	const	float3	pe_scale	= {1.f,		128.f,		16384.f		};
static	const 	float3 	pe_unscale21 	= {2.f/1.f,	2.f/128.f,	2.f/16384.f	};
static	const 	float3 	pe_unscale24 	= {1.f/1.f,	1.f/256.f,	1.f/65536.f	};
float 	decode_float21	( float3 rgb)	{	return 	dot	( rgb,	pe_unscale21);	}
float 	decode_float24	( float3 rgb)	{	return 	dot	( rgb,	pe_unscale24);	}


float 	calc_cyclic 	(float x)				{
	float 	phase 	= 1/(2*3.141592653589f);
	float 	sqrt2	= 1.4142136f;
	float 	sqrt2m2	= 2.8284271f;
	float 	f 	= sqrt2m2*frac(x)-sqrt2;	// [-sqrt2 .. +sqrt2] !No changes made, but this controls the grass wave (which is violent if I must say)
	return 	f*f - 1.f;				// [-1     .. +1]
}
float2 	calc_xz_wave 	(float2 dir2D, float frac)		{
	// Beizer
	float2  ctrl_A	= float2(0.f,		0.f	);
	float2 	ctrl_B	= float2(dir2D.x,	dir2D.y	);
	return  lerp	(ctrl_A, ctrl_B, frac);			//!This calculates tree wave. No changes made
}


#endif
