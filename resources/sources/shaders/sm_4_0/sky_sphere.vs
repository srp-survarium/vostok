////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-18 from the shipped v0.100b blob
//	(shaders/sm_4_0/sky_sphere.vs, one permutation) - absent from the
//	recovered source drop.  Byte-identity is the acceptance test:
//	python3 -m vostok.shaders roundtrip sky_sphere.vs
//
//	The sky sphere's transform, and the one thing it does beyond a world-
//	view-projection: it spins the sphere about Y by an angle the engine
//	hands over pre-evaluated, as the (cos, sin) pair sky_cos_sin.  The sky
//	turns with the weather, so the rotation is a matrix composed *before*
//	the projection rather than a texture-coordinate offset - which is why
//	the blob is 38 slots of dp2/dp3/dp4 instead of the usual four dp4s: fxc
//	folds the whole m_VP * (m_W * R) product per output component, and the
//	zeros in R collapse three of its four columns into dp2s.
//
//	m_W is a float3x4, so the world matrix is its three rows plus the
//	implied (0,0,0,1); that bottom row is what turns the composed matrix's
//	first three columns into dp3s and leaves only the translation column a
//	dp4.
////////////////////////////////////////////////////////////////////////////

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
