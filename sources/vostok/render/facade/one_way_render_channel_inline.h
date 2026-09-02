// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_ONE_WAY_RENDER_CHANNEL_INLINE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_ONE_WAY_RENDER_CHANNEL_INLINE_H_INCLUDED
namespace vostok {
namespace render {

inline one_way_render_channel::one_way_render_channel(
	memory::base_allocator& owner_allocator
)	:
	m_channel						( owner_allocator ),
	m_current_frame_id				( 0 ),
	m_process_next_frame_commands	( false )
{
	render_initialize	( );
}

inline void one_way_render_channel::owner_initialize( )
{
	m_channel.owner_initialize(
		VOSTOK_NEW_IMPL( m_channel.owner_allocator( ), null_render_command ),
		VOSTOK_NEW_IMPL( m_channel.owner_allocator( ), null_render_command )
	);
}

inline void one_way_render_channel::owner_finalize( )
{
	m_channel.owner_finalize( m_next_frame_commands_queue );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_ONE_WAY_RENDER_CHANNEL_INLINE_H_INCLUDED
