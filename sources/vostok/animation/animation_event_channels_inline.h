////////////////////////////////////////////////////////////////////////////
//	Created		: 03.02.2011
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_EVENT_CHANNELS_INLINE_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_EVENT_CHANNELS_INLINE_H_INCLUDED

namespace vostok {
namespace animation {

inline size_t		channels_self_memory_size( u32 channels_cnt )
{
	return channels_cnt * sizeof( event_channel );
}

template	<class ConfigValueType>
inline size_t	animation_event_channels::count_internal_memory_size( ConfigValueType const& config )
{
	size_t mem_size = channels_self_memory_size ( config.size() );
	
	typename ConfigValueType::const_iterator i = config.begin(), e = config.end();
	
	for ( ; i!=e; ++i )
		mem_size += event_channel::count_internal_memory_size( *i );
	return mem_size;

}

template	<class ConfigValueType>
inline void	animation_event_channels::create_in_place_internals ( ConfigValueType const& config, void* memory_buff )
{
	u32 const cnt = config.size();

	m_channels_count = cnt;

	if ( cnt == 0 )
		return;

	m_internal_memory_position = (u32)bytes_dist(  memory_buff, this );
	
	for ( u32 i = 0; i < m_channels_count; ++i )
	{
		new ( &channel( i ) ) event_channel;
	}
	
	memory_buff = get_shift_ptr( memory_buff, (u32)channels_self_memory_size( m_channels_count ) );

	for ( u32 i = 0; i < m_channels_count; ++i )
	{
		channel( i ).create_in_place_internals( config[ i ], memory_buff );
		
		memory_buff = get_shift_ptr( memory_buff, event_channel::count_internal_memory_size( config[i] ) );
	}
}

#ifndef	MASTER_GOLD

inline void	animation_event_channels::write( configs::lua_config_value &cfg )const
{
	if ( m_channels_count == u32(-1) )
		return;

	for ( u32 i = 0; i < m_channels_count; ++i )
	{
		configs::lua_config_value channel_cfg = cfg[i];
		channel( i ).write( channel_cfg );
	}
}

#endif

inline u32 animation_event_channels::count_internal_memory_size(
	animation_event_channels const& event_channels
)
{
	if ( event_channels.channels_count() == u32(-1) )
		return 0;

	u32 memory_size = u32( channels_self_memory_size( event_channels.channels_count() ) );

	for ( u32 i = 0; i < event_channels.channels_count(); ++i )
		memory_size += event_channel::count_internal_memory_size( event_channels.channel( i ) );

	return memory_size;
}

inline size_t animation_event_channels::internal_memory_size( ) const
{
	return count_internal_memory_size( *this );
}

inline void animation_event_channels::copy_internals( void* memory_buffer ) const
{
	memory::copy(
		memory_buffer,
		internal_memory_size(),
		(void const*) ( pbyte( this ) + m_internal_memory_position ),
		internal_memory_size()
	);
}

inline void animation_event_channels::create_in_place_internals(
	animation_event_channels const& event_channels,
	void* memory_buffer
)
{
	m_internal_memory_position = u32( pbyte( memory_buffer ) - pbyte( this ) );
	m_channels_count = event_channels.channels_count();

	event_channels.copy_internals( memory_buffer );
}

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_EVENT_CHANNELS_INLINE_H_INCLUDED
