#include "common.h"

struct v_rain
{
	float4	position	: POSITION;
	float2	tc			: TEXCOORD0;
};

struct v2p_rain
{
	float4	hpos			: SV_Position;
	float3	world_position	: TEXCOORD0;
	float3	view_position	: TEXCOORD1;
	float2	tc				: TEXCOORD2;
};

v2p_rain main( v_rain I )
{
	v2p_rain	O;

	float4	world		= float4( mul( m_W, float4( I.position.xyz, 1.0f)), 1.0f);

	O.hpos				= mul( m_VP, world);
	O.world_position	= world.xyz;
	O.view_position		= mul( m_V, world);
	O.tc				= I.tc;

	return	O;
}
