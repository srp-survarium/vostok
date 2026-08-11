////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/animation_event_channels.h>
#include "bi_spline_event_channel_baked.h"

namespace vostok {
namespace animation {

u32 animation_event_channels::count_internal_memory_size(
	bi_spline_event_channel_baked const* channels,
	u32 channels_count
)
{
	u32 size = u32( channels_self_memory_size( channels_count ) );

	for ( u32 i = 0; i != channels_count; ++i )
		size += event_channel::count_internal_memory_size( channels[i] );

	return size;
}

void animation_event_channels::create_in_place_internals(
	bi_spline_event_channel_baked const* channels,
	u32 channels_count,
	void* memory_buffer
)
{
	m_channels_count = channels_count;
	m_internal_memory_position = u32( bytes_dist( memory_buffer, this ) );

	for ( u32 i = 0; i < m_channels_count; ++i )
		new ( &channel( i ) ) event_channel;

	memory_buffer = get_shift_ptr(
		memory_buffer,
		u32( channels_self_memory_size( m_channels_count ) )
	);

	for ( u32 i = 0; i < m_channels_count; ++i ) {
		channel( i ).create_in_place_internals( channels[i], memory_buffer );
		memory_buffer = get_shift_ptr(
			memory_buffer,
			event_channel::count_internal_memory_size( channels[i] )
		);
	}
}

event_channel			&animation_event_channels::channel( u32 id )
{
	ASSERT( m_channels_count != 0 );
	ASSERT( m_channels_count != u32(-1) );
	ASSERT( id < m_channels_count );
	return get_shift_ptr( (event_channel*) (this), m_internal_memory_position )[id];
}

event_channel	const	&animation_event_channels::channel( u32 id ) const
{
	ASSERT( m_channels_count != 0 );
	ASSERT( m_channels_count != u32(-1) );
	ASSERT( id < m_channels_count );
	return get_shift_ptr( (event_channel const*) (this), m_internal_memory_position )[id];
}

struct find_predicate
{
	find_predicate( pcstr name ): m_name( name ){}

	bool operator () ( const event_channel &channel )
	{
		return strings::equal( channel.name(), m_name );
	}

	pcstr m_name;
};

STATIC_SIZE_ASSERT(find_predicate, 0x4);

u32			animation_event_channels::get_channel_id( pcstr name ) const
{
	if ( m_channels_count == u32(-1) || m_channels_count == 0 )
		return u32(-1);

	const event_channel  *begin = &channel( 0 ), *end = ( &channel(m_channels_count-1) + 1 );

	find_predicate p( name );

	const event_channel  * r = std::find_if ( begin, end,  p );

	if ( r == end )
		return u32(-1);

	return u32( r - begin );

}

animation_event_channels::animation_event_channels():
m_channels_count( u32(-1) ),
m_internal_memory_position( size_t(-1) )
{

}

animation_event_channels::~animation_event_channels()
{
	if ( m_channels_count ==  u32(-1) || m_channels_count == 0 )
		return;
	ASSERT( m_channels_count !=  u32(-1) );
	ASSERT( m_internal_memory_position != size_t(-1) );
	for ( u32 i = 0; i < m_channels_count; ++i )
		channel( i ).~event_channel();
}

} // namespace animation
} // namespace vostok
