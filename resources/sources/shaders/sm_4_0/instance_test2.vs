#include "common.h"

uniform float4x4	light_instances[128];

struct v_in
{
	float3	position	: POSITION;
	float	temp		: TempFloat;
	uint	instance	: SV_InstanceID;
};

float4 main( v_in I, out float3 light_color : LightColor, out float light_range : LightRange, out float3 light_position : LightPosition ) : SV_Position
{
	float4	row0	= light_instances[I.instance][0];
	float4	row1	= light_instances[I.instance][1];

	float3	position	= I.position*row0.w + row0.xyz;

	light_color			= row1.xyz*row1.w;
	light_range			= row0.w;
	light_position		= row0.xyz;

	return	mul( m_VP, float4( position, 1.0f));
}
