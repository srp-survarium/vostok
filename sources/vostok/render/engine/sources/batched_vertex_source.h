// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_BATCHED_VERTEX_SOURCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_BATCHED_VERTEX_SOURCE_H_INCLUDED
#include <vostok/math_color.h>

namespace vostok {
namespace render {

struct batched_vertex_source {
	float3		position;
	math::color	normal;
	math::color	tangent;
	math::color	binormal;
	math::color	clr;
	float2		uv;
};

STATIC_SIZE_ASSERT( batched_vertex_source, 0x24 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_BATCHED_VERTEX_SOURCE_H_INCLUDED
