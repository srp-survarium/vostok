////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_BI_SPLINE_CHANNEL_ANIMATION_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_CHANNEL_ANIMATION_BAKED_H_INCLUDED

namespace vostok {
namespace animation {

class bi_spline_channel_animation_baked {
public:
	inline	u32									knots_count		( ) const { /* no source */ }

	inline	float								knot			( const u32 arg_0 ) const { /* no source */ }

	inline	float								point			( const u32 arg_0 ) const { /* no source */ }

	inline	std::pair< float, float > const*	data			( ) const { /* no source */ }

	inline	u32									points_number	( ) const { /* no source */ }

private:
	/* 0x0000 */	u32		m_knots_count;
}; // class bi_spline_channel_animation_baked

STATIC_SIZE_ASSERT(bi_spline_channel_animation_baked, 0x4);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_CHANNEL_ANIMATION_BAKED_H_INCLUDED
