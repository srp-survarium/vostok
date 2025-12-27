////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED

/* INCLUDES */
struct vostok::platform_pointer_selector<float,1>::helper;
enum vostok::animation::channel_type_enum;

namespace vostok {
namespace animation {

class bi_spline_event_channel_baked {
public:
	inline	float const*			knots							( ) const { /* no source */ }

	inline	u32						knots_count						( ) const { /* no source */ }

	inline	pcbyte					domains							( ) const { /* no source */ }

	inline	u32						domains_count					( ) const { /* no source */ }

	inline	channel_type_enum		type							( ) const { /* no source */ }

	inline	pcstr					name							( ) const { /* no source */ }

	inline							bi_spline_event_channel_baked	( ) { /* no source */ }

private:
	/* 0x0000 */	platform_pointer_selector< float, 1 >::helper	m_knots;
	/* 0x0008 */	u16		m_knots_count;
	/* 0x000a */	u8		m_domains_count;
	/* 0x000b */	u8		m_type;
}; // class bi_spline_event_channel_baked

STATIC_SIZE_ASSERT(bi_spline_event_channel_baked, 0x10);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED
