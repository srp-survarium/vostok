////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/event_channel.h>
#include "bi_spline_event_channel_baked.h"

namespace vostok {
namespace animation {

u32 event_channel::count_internal_memory_size(
	bi_spline_event_channel_baked const& channel
)
{
	return channel.knots_count() ?
		time_channel_type::count_internal_memory_size( channel.knots_count() ) :
		0;
}

void event_channel::create_in_place_internals(
	bi_spline_event_channel_baked const& channel,
	void* memory_buffer
)
{
	u32 const count = channel.knots_count();

	strings::copy( m_name, channel.name() );
	m_type = channel.type();
	m_time_channel.create_in_place( memory_buffer, count );

	for ( u32 i = 0; i < count; ++i )
		m_time_channel.knot( i ) = channel.knots()[i];

	for ( u32 i = 0; i < count; ++i )
		reinterpret_cast< domain_data* >( memory_buffer )[i].data = channel.domains()[i];
}

u32		event_channel::internal_memory_size	()const
{
	return time_channel_type::count_internal_memory_size( m_time_channel.knots_count() );
}



} // namespace animation
} // namespace vostok
