#include "common.h"

uniform int		rsm_size;
uniform float4	grid_origin_and_inv_grid_scale;

struct v_in
{
	float3	position	: POSITION;
};

struct v2g
{
	float4	hpos		: POSITION;
	float3	normal		: NORMAL;
	float3	flux		: COLOR;
	float3	world		: TEXCOORD0;
};

v2g main( v_in I )
{
	v2g		output;

	float3	world		= mul( m_W, float4( I.position.xzy, 1.0f));

	output.hpos			= float4( ( world - grid_origin_and_inv_grid_scale.xyz)*grid_origin_and_inv_grid_scale.w, 1.0f);

	float3	origin		= float3( m_W[0].w, m_W[1].w, m_W[2].w);
	output.normal		= normalize( world - origin);
	output.flux			= float3( 0.0f, 0.0f, 0.0f);
	output.world		= world;

	return	output;
}
