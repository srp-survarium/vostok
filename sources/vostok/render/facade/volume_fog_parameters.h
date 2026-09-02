// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

struct volume_fog_parameters {
	inline	volume_fog_parameters	( );

	float4x4	transform;
	float3		fog_color;
	float2		direction;
	float		height_falloff_offset;
	float		density;
	float		speed;
	float		noise_scale;
	float		wave_length;
	float		near_density;
	float		transparency_multiplier;
	float		density_offset;
};

STATIC_SIZE_ASSERT( volume_fog_parameters, 0x74 );

namespace
{
	static float clear_value	= 1.0f;
} // namespace

inline volume_fog_parameters::volume_fog_parameters( )
{
	transform					= vostok::math::float4x4().identity();
	fog_color					= float3( clear_value, clear_value, clear_value );
	density						= clear_value;
	speed						= clear_value;
	noise_scale					= clear_value;
	wave_length					= clear_value;
	near_density				= 0.0f;
	density_offset				= 0.0f;
	height_falloff_offset		= 0.0f;
	transparency_multiplier		= clear_value;
	direction					= float2( clear_value, 0.0f );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
