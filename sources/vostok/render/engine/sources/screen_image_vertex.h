// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SCREEN_IMAGE_VERTEX_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCREEN_IMAGE_VERTEX_H_INCLUDED
#include <vostok/math_float2.h>

namespace vostok {
namespace render {

struct screen_image_vertex {
	screen_image_vertex( float x, float y, float u, float v ) :
		position	( x, y ),
		text_coords	( u, v )
	{
	}

public:
	float2 position;
	float2 text_coords;
};

STATIC_SIZE_ASSERT( screen_image_vertex, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCREEN_IMAGE_VERTEX_H_INCLUDED
