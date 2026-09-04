// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED
#define ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED

#include <vostok/platform_pointer.h>
#include <vostok/animation/event_channel.h>

namespace vostok {
namespace animation {

class bi_spline_event_channel_baked {
public:
	inline	float const*			knots							( ) const { return m_knots; }

	inline	u32						knots_count						( ) const { return m_knots_count; }

	inline	pcbyte					domains							( ) const
	{
		return reinterpret_cast< pcbyte >( knots() + knots_count() );
	}

	inline	u32						domains_count					( ) const { return m_domains_count; }

	inline	channel_type_enum		type							( ) const
	{
		return static_cast< channel_type_enum >( m_type );
	}

	inline	pcstr					name							( ) const
	{
		return reinterpret_cast< pcstr >( domains() + domains_count() );
	}


private:
	/* 0x0000 */	platform_pointer_selector< float, platform_pointer_64bit >::helper	m_knots;
	/* 0x0008 */	u16		m_knots_count;
	/* 0x000a */	u8		m_domains_count;
	/* 0x000b */	u8		m_type;

	friend void create_baked_animation_in_place( void* const raw_buffer, const u32 buffer_size );
}; // class bi_spline_event_channel_baked

STATIC_SIZE_ASSERT(bi_spline_event_channel_baked, 0x10);

} // namespace animation
} // namespace vostok

#include <vostok/animation/event_channel_inline.h>

#endif // #ifndef ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED
