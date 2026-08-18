#include "common.h"

uniform float4	render_target_size;

struct v_in
{
	float4	data	: POSITION;
	float2	cell	: TEXCOORD0;
};

float4 main( v_in I, out float4 data : TEXCOORD0 ) : SV_Position
{
	data		= I.data;

	float2	uv	= ( I.cell + 1.0f)/render_target_size.xy;

	return	float4( uv.x*2.0f - 1.0f, uv.y*-2.0f + 1.0f, 0.0f, 1.0f);
}
