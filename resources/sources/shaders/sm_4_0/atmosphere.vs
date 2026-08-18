#include "common.h"

struct v_in
{
	float4	position	: POSITION;
	float2	tc			: TEXCOORD0;
};

void main( in v_in I, out float4 hpos : SV_Position, out float3 world_position : TEXCOORD0, out float3 object_position : TEXCOORD1, out float2 tc : TEXCOORD2 )
{
	float4	position	= float4( I.position.xyz*100000.0f + float3( 0.0f, 77500.0f, 0.0f), 1.0f);
	float4	world		= float4( mul( m_W, position), 1.0f);

	hpos				= mul( m_VP, world);
	world_position		= world.xyz;
	object_position		= I.position.xyz;
	tc					= I.tc;
}
