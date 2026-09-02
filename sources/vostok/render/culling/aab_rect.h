// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CULLING_AAB_RECT_H_INCLUDED
#define VOSTOK_RENDER_CULLING_AAB_RECT_H_INCLUDED
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>

namespace vostok {
namespace render {
namespace culling {

class aab_rect {
public:
	float2 min;
	float2 max;

	inline aab_rect& modify( float3 const& point );
	inline aab_rect& modify( float2 const& point );
	inline bool intersects( aab_rect const& other ) const;
	inline bool contains( aab_rect const& another ) const;

private:
	inline aab_rect( );

	friend class portal_sector_system;
	friend aab_rect get_intersection_rect( aab_rect const& left, aab_rect const& right );
};

STATIC_SIZE_ASSERT( aab_rect, 0x10 );

} // namespace culling
} // namespace render
} // namespace vostok

#include <vostok/render/engine/sources/aab_rect_inline.h>

#endif // VOSTOK_RENDER_CULLING_AAB_RECT_H_INCLUDED
