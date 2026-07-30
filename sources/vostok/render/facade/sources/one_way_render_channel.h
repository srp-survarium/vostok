#ifndef VOSTOK_RENDER_FACADE_SOURCES_ONE_WAY_RENDER_CHANNEL_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_ONE_WAY_RENDER_CHANNEL_H_INCLUDED

#include <vostok/intrusive_list.h>
#include <vostok/intrusive_spsc_queue.h>
#include <vostok/one_way_threads_channel.h>
#include <vostok/threading_event.h>
#include <vostok/render/api.h>
#include <vostok/render/facade/base_command.h>

namespace vostok {
namespace render {

typedef intrusive_list<
	base_command,
	base_command*,
	&base_command::deferred_next,
	threading::single_threading_policy
> base_command_list;

class one_way_render_channel : public core::noncopyable {
private:
	typedef intrusive_spsc_queue<
		base_command,
		base_command,
		&base_command::next
	> render_commands_queue_type;
	typedef one_way_threads_channel<
		render_commands_queue_type,
		render_commands_queue_type
	> one_way_threads_channel_type;

public:
	explicit one_way_render_channel( memory::base_allocator& owner_allocator )
		: m_channel( owner_allocator ),
		  m_current_frame_id( 0 ),
		  m_process_next_frame_commands( false )
	{
		render_initialize( );

		// FUNCTION BODY[0x15b10]
	}

	void owner_initialize( )
	{
		m_channel.owner_initialize(
			VOSTOK_NEW_IMPL( m_channel.owner_allocator( ), null_render_command ),
			VOSTOK_NEW_IMPL( m_channel.owner_allocator( ), null_render_command )
		);

		// FUNCTION BODY[0x12c20]
	}

	void owner_finalize( )
	{
		m_channel.owner_finalize( m_next_frame_commands_queue );

		// FUNCTION BODY[0x12cb0]
	}

	void owner_push_back( base_command* value )
	{
		bool const was_empty = m_channel.user_is_queue_empty( );
		m_channel.owner_push_back( value );
		if ( was_empty )
			m_wait_form_command_event.set( true );
	}

	void owner_delete_processed_items( )
	{
		m_channel.owner_delete_processed_items( );
	}

	bool render_process_commands( bool wait_for_command_if_queue_is_empty );
	void render_on_draw_scene(
		base_scene_ptr const& scene,
		base_scene_view_ptr const& scene_view,
		bool use_depth
	);
	void render_on_draw_scene2( base_scene_ptr*, base_scene_view_ptr*, bool ) { }
	void render_on_end_frame( );

private:
	void render_initialize( )
	{
		m_channel.user_initialize( );
	}

	bool render_is_queue_empty( ) const
	{
		return m_channel.user_is_queue_empty( );
	}

	void process_next_frame_commands( );

	template < typename scene_pointer_type >
	void move_commands_from_list( scene_pointer_type const& )
	{
		// STATE[STUB]
		// FUNCTION BODY[0x15c50] for base_scene_ptr
	}

private:
	struct null_render_command : base_command {
		virtual void execute( ) { }
		null_render_command( ) { }
		virtual ~null_render_command( ) { }
	};

private:
	one_way_threads_channel_type m_channel;
	threading::event m_wait_form_command_event;
	base_command_list m_next_frame_commands_queue;
	base_scene_ptr m_scenes;
	base_scene_view_ptr m_scene_views;
	u32 m_current_frame_id;
	bool m_process_next_frame_commands;

public:
	~one_way_render_channel( ) { }
};

STATIC_SIZE_ASSERT( one_way_render_channel, 0xb8 );
STATIC_SIZE_ASSERT( one_way_render_channel::null_render_command, 0x54 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_ONE_WAY_RENDER_CHANNEL_H_INCLUDED
