// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_ENVIRONMENT_TEMP_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_ENVIRONMENT_TEMP_H_INCLUDED
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/render/facade/cloud_key.h>

namespace vostok {
namespace render {

struct environment_temp {
	environment_temp( );
	~environment_temp( );

	void tick( float )
	{
		// STATE[STUB]
	}

	cloud_key_parameters get_interp_key( float time );
	cloud_key_parameters get_next_key( u32 const index );

	float3 get_sun_direction( float )
	{
		// STATE[STUB]
		return float3( 0.0f, 0.0f, 0.0f );
	}

public:
	cloud_key_parameters*	keys;
	float					key_time_step;
	u32						num_keys;
	float4					sky_light[5];
	float					time;
};

STATIC_SIZE_ASSERT( environment_temp, 0x60 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_ENVIRONMENT_TEMP_H_INCLUDED
