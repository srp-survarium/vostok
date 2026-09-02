// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_RAY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RAY_H_INCLUDED
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct ray {
	ray( )
	{
	}

	ray( float3 const& position, float3 const& in_direction ) :
		direction	( in_direction ),
		origin		( position )
	{
	}

	float3	direction;
	float3	origin;
};

STATIC_SIZE_ASSERT( ray, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RAY_H_INCLUDED
