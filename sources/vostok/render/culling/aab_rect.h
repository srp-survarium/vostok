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

	aab_rect& modify( float3 const& point )
	{
		return modify( float2( point.x, point.y ) );
	}

	aab_rect& modify( float2 const& point )
	{
		min.x = min.x < point.x ? min.x : point.x;
		min.y = min.y < point.y ? min.y : point.y;
		max.x = max.x > point.x ? max.x : point.x;
		max.y = max.y > point.y ? max.y : point.y;
		return *this;
	}

	bool intersects( aab_rect const& other ) const
	{
		return min.x <= other.max.x && max.x >= other.min.x &&
			min.y <= other.max.y && max.y >= other.min.y;
	}

	bool contains( aab_rect const& other ) const
	{
		// FUNCTION BODY[0x8a470]
		return min.x <= other.min.x && min.y <= other.min.y &&
			max.x >= other.max.x && max.y >= other.max.y;
	}

private:
	aab_rect( )
	{
		min = float2( -1.f, -1.f );
		max = float2( 1.f, 1.f );
	}

	friend class portal_sector_system;
	friend aab_rect get_intersection_rect( aab_rect const& left, aab_rect const& right );
};

STATIC_SIZE_ASSERT( aab_rect, 0x10 );

inline aab_rect get_intersection_rect( aab_rect const& left, aab_rect const& right )
{
	// FUNCTION BODY[0x8a530]
	aab_rect result;
	result.min.x = left.min.x > right.min.x ? left.min.x : right.min.x;
	result.min.y = left.min.y > right.min.y ? left.min.y : right.min.y;
	result.max.x = left.max.x < right.max.x ? left.max.x : right.max.x;
	result.max.y = left.max.y < right.max.y ? left.max.y : right.max.y;
	return result;
}

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_CULLING_AAB_RECT_H_INCLUDED
