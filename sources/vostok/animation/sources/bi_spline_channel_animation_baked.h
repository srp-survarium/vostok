////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_BI_SPLINE_CHANNEL_ANIMATION_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_CHANNEL_ANIMATION_BAKED_H_INCLUDED

namespace vostok {
namespace animation {

class bi_spline_channel_animation_baked {
public:
	inline	u32									knots_count		( ) const { return m_knots_count; }

	inline	float								knot			( u32 const index ) const
	{
		u32 const actual_index = index < points_number() ? index : points_number() - 1;
		return data()[ actual_index ].first;
	}

	inline	float								point			( u32 const index ) const
	{
		u32 const actual_index = index < points_number() ? index : points_number() - 1;
		return data()[ actual_index ].second;
	}

	inline	std::pair< float, float > const*	data			( ) const
	{
		return reinterpret_cast< std::pair< float, float > const* >( this + 1 );
	}

	inline	u32									points_number	( ) const { return m_knots_count; }

private:
	/* 0x0000 */	u32		m_knots_count;
}; // class bi_spline_channel_animation_baked

STATIC_SIZE_ASSERT(bi_spline_channel_animation_baked, 0x4);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_CHANNEL_ANIMATION_BAKED_H_INCLUDED
