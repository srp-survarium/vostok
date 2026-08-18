#include "common.h"

uniform float4	to_sun_direction_parameter;
uniform float4	sky_clouds_parameters2;	// x - sin, y - cos of the cloud layer rotation

struct v_in
{
	float4	position	: POSITION;
	float2	tc			: TEXCOORD0;
};

void main( in v_in I, out float4 hpos : SV_Position, out float3 world_position : TEXCOORD0, out float3 object_position : TEXCOORD1, out float2 tc : TEXCOORD2 )
{
	float4	position	= float4( I.position.xyz*100000.0f, 1.0f);
	float2	rotation	= sky_clouds_parameters2.yx*float2( 1.0f, -1.0f);

	float4	row_x		= float4( dot( m_W[0].xz, rotation), m_W[0].y, dot( m_W[0].xz, sky_clouds_parameters2.xy), m_W[0].w);
	float4	row_y		= float4( dot( m_W[1].xz, rotation), m_W[1].y, dot( m_W[1].xz, sky_clouds_parameters2.xy), m_W[1].w);
	float4	row_z		= float4( dot( m_W[2].xz, rotation), m_W[2].y, dot( m_W[2].xz, sky_clouds_parameters2.xy), m_W[2].w);

	float4	world		= float4( dot( row_x, position), dot( row_y, position), dot( row_z, position), 1.0f);

	hpos				= mul( m_VP, world);
	world_position		= world.xyz;
	object_position		= I.position.xyz;
	tc					= I.tc;
}
