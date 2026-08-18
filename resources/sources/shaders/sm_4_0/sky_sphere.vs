#include "common.h"

uniform float4	sky_cos_sin;

struct v2p_sky_sphere
{
	float4	hpos	: SV_Position;
	float2	tc		: TEXCOORD0;
};

struct v_sky_sphere
{
	float4	position	: POSITION;
	float2	tc			: TEXCOORD0;
};

v2p_sky_sphere main( v_sky_sphere I )
{
	v2p_sky_sphere	O;

	float4x4	world		= float4x4( m_W[0], m_W[1], m_W[2], float4( 0.0f, 0.0f, 0.0f, 1.0f));

	float4x4	rotation	= float4x4(
		 sky_cos_sin.x,	0.0f,	sky_cos_sin.y,	0.0f,
		 0.0f,			1.0f,	0.0f,			0.0f,
		-sky_cos_sin.y,	0.0f,	sky_cos_sin.x,	0.0f,
		 0.0f,			0.0f,	0.0f,			1.0f
	);

	O.hpos	= mul( mul( m_VP, mul( world, rotation)), float4( I.position.xyz, 1.0f));
	O.tc	= I.tc;

	return	O;
}
