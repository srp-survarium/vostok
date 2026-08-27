#ifndef VOSTOK_RENDER_ENGINE_AAB_RECT_INLINE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_AAB_RECT_INLINE_H_INCLUDED

namespace vostok {
namespace render {
namespace culling {

inline aab_rect& aab_rect::modify( float3 const& point )
{
	return modify( float2( point.x, point.y ) );
}

inline aab_rect& aab_rect::modify( float2 const& point )
{
	min.x = min.x < point.x ? min.x : point.x;
	min.y = min.y < point.y ? min.y : point.y;
	max.x = max.x > point.x ? max.x : point.x;
	max.y = max.y > point.y ? max.y : point.y;
	return *this;
}

inline bool aab_rect::intersects( aab_rect const& other ) const
{
	return min.x < other.max.x && max.x > other.min.x &&
		min.y < other.max.y && max.y > other.min.y;
}

inline bool aab_rect::contains( aab_rect const& another ) const
{
	return ( min.x <= another.min.x || math::is_similar( min.x, another.min.x ) ) &&
		( max.x >= another.max.x || math::is_similar( max.x, another.max.x ) ) &&
		( min.y <= another.min.y || math::is_similar( min.y, another.min.y ) ) &&
		( max.y >= another.max.y || math::is_similar( max.y, another.max.y ) );
}

inline aab_rect::aab_rect( )
{
	min.set( -1.f, -1.f );
	max.set( 1.f, 1.f );
}

inline aab_rect get_intersection_rect( aab_rect const& left, aab_rect const& right )
{
	aab_rect result;

	result.min.set( 0.f, 0.f ), result.max.set( 0.f, 0.f );
	result.min.x = std::max( left.min.x, right.min.x );
	result.min.y = std::max( left.min.y, right.min.y );
	result.max.x = std::min( left.max.x, right.max.x );
	result.max.y = std::min( left.max.y, right.max.y );

	return result;
}

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_AAB_RECT_INLINE_H_INCLUDED
