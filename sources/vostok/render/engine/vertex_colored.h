// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_VERTEX_COLORED_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_VERTEX_COLORED_H_INCLUDED
#include <vostok/math_color.h>
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct vertex_colored {
	vertex_colored( ) { }

	vertex_colored( float3 const& position, math::color const& color )
		: position( position ),
		  color( color )
	{
	}

	float3 position;
	math::color color;
};

STATIC_SIZE_ASSERT( vertex_colored, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_VERTEX_COLORED_H_INCLUDED
