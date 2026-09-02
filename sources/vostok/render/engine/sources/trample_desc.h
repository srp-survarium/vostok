// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_TRAMPLE_DESC_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_TRAMPLE_DESC_H_INCLUDED
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct trample_desc {
	float3	position;
	float	radius;
	float	multiplier;
};

STATIC_SIZE_ASSERT( trample_desc, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_TRAMPLE_DESC_H_INCLUDED
