// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_LPV_VERTEX_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LPV_VERTEX_H_INCLUDED
#include <vostok/math_color.h>
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct lpv_vertex {
	float3		position;
	math::color	normal;
	math::color	clr;
};

STATIC_SIZE_ASSERT( lpv_vertex, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_LPV_VERTEX_H_INCLUDED
