////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-18 from the shipped v0.100b blob
//	(shaders/sm_4_0/rain.vs, one permutation) - absent from the recovered
//	source drop.  Byte-identity is the acceptance test:
//	python3 -m vostok.shaders roundtrip rain.vs
//
//	The rain volume's transform.  It hands the pixel stage three things:
//	the clip position, the world position (rain.ps reprojects it into the
//	rain shadow map) and the view position, of which rain.ps reads only .z -
//	the pixel's own depth, which it compares against the g-buffer's to fade
//	drops that are behind geometry.  m_V is applied here rather than there
//	because the world position is needed anyway and one dp4 in the vertex
//	stage is cheaper than a matrix in the pixel stage.
//
//	No $Globals: nothing is declared loosely, so the first cbuffer slot goes
//	to dynamic_transforms and the second to static_globals.
////////////////////////////////////////////////////////////////////////////

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
