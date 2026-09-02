// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SHADOW_VERTEX_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SHADOW_VERTEX_H_INCLUDED
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct shadow_vertex {
	float3	position;
	float3	object_position;
	float2	uv;
};

STATIC_SIZE_ASSERT( shadow_vertex, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SHADOW_VERTEX_H_INCLUDED
