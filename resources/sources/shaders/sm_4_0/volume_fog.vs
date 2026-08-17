////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 from the shipped v0.100b blob - absent from the
//	recovered source drop. Byte-identity: python3 -m vostok.shaders roundtrip
////////////////////////////////////////////////////////////////////////////

#include "common.h"

uniform float4x4	inverted_world_matrix;
uniform float3		eye_pos_os;
uniform float3		eye_pos_ws;
uniform int			is_inside_volume;

struct v_in
{
	float3	position	: POSITION;
};

float4 main( v_in I, out float3 world_delta : TEXCOORD0, out float3 object_delta : TEXCOORD1, out float3 box_max : TEXCOORD2, out float3 box_min : TEXCOORD3 ) : SV_Position
{
	float4	position	= float4( I.position*2.0f, 1.0f);

	float4	hpos		= mul( m_WVP, position);
	hpos.z				= hpos.w*0.01f;

	float3	world		= mul( m_W, position);
	world_delta			= world - eye_pos_ws;
	object_delta		= mul( (float3x3)inverted_world_matrix, world_delta);
	box_max				= 1.0f - eye_pos_os;
	box_min				= -1.0f - eye_pos_os;

	return	hpos;
}
