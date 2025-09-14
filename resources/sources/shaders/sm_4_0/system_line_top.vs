#include "common.h"

struct	v2p_TL0uv_SL
{
	float4	Color			: COLOR;
	float4 	HPos			: SV_Position;	// Clip-space position 	(for rasterization)
	float4	LineStartP		: TEXCOORD0;
	float	Shift			: TEXCOORD1;
	uint	Pattern			: TEXCOORD2;
};

struct	v_TL0uv_SL
{
	float3	P				: POSITION;
	float4	Color			: COLOR; 
	float3	LineStartP		: TEXCOORD0;
	float	Shift			: TEXCOORD1;
	uint	Pattern			: TEXCOORD2;
};

//#define LINE3D 1

float4x4 m_WVP_sl;

v2p_TL0uv_SL main ( v_TL0uv_SL I) 
{
	v2p_TL0uv_SL vout;
	
	vout.HPos 		= mul( m_WVP_sl, float4(I.P,1));
	vout.HPos.z		= 0;
	vout.LineStartP	= mul( m_WVP_sl, float4(I.LineStartP,1));
	
	vout.Shift 		= I.Shift;
	vout.Pattern 	= I.Pattern;
	vout.Color 		= I.Color.bgra;

 	return vout;
}

