////////////////////////////////////////////////////////////////////////////
//	Created		: 04.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ONE_WAY_RENDER_CHANNEL_H_INCLUDED
#define VOSTOK_RENDER_ONE_WAY_RENDER_CHANNEL_H_INCLUDED

#include <vostok/one_way_threads_channel.h>
#include <vostok/render/facade/base_command.h>
#include <vostok/render/api.h>
#include <vostok/intrusive_spsc_queue.h>

template class VOSTOK_RENDER_API
	vostok::intrusive_list<
		vostok::render::base_command,
		vostok::render::base_command*,
		&vostok::render::base_command::deferred_next,
		vostok::threading::single_threading_policy
	>;

namespace vostok {
namespace render {

typedef intrusive_list<
	base_command,
	base_command*,
	&base_command::deferred_next,
	threading::single_threading_policy
> base_command_list;

class VOSTOK_RENDER_API one_way_render_channel : public core::noncopyable {
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
	inline	explicit	one_way_render_channel		( memory::base_allocator& owner_allocator );

	inline	void		owner_initialize			( );
	inline	void		owner_finalize				( );
	inline	void		owner_push_back				( base_command* value );
	inline	void		owner_delete_processed_items( );

			bool		render_process_commands		( bool wait_for_command_if_queue_is_empty );
			void		render_on_draw_scene		(
							base_scene_ptr const&		scene,
							base_scene_view_ptr const&	scene_view,
							bool						use_depth
						);
	inline	void		render_on_draw_scene2		(
							base_scene_ptr*				scene,
							base_scene_view_ptr*			scene_view,
							bool						use_depth
						)
	{
		VOSTOK_UNREFERENCED_PARAMETERS	( scene, scene_view, use_depth );
	}
			void		render_on_end_frame			( );

private:
	inline	void		render_initialize			( );
	inline	bool		render_is_queue_empty		( ) const;
			void		process_next_frame_commands	( );

	template < typename Scene_OR_SceneViewType >
	inline	void		move_commands_from_list		( Scene_OR_SceneViewType const& scene_or_scene_view );

private:
	struct null_render_command : base_command {
		virtual	void	execute				( ) { }
		inline			null_render_command	( ) { }
		virtual			~null_render_command( ) { }
	}; // struct null_render_command

private:
	/* 0x0000 */	one_way_threads_channel_type	m_channel;
	/* 0x0090 */	threading::event				m_wait_form_command_event;
	/* 0x0098 */	base_command_list			m_next_frame_commands_queue;
	/* 0x00a8 */	base_scene_ptr				m_scenes;
	/* 0x00ac */	base_scene_view_ptr			m_scene_views;
	/* 0x00b0 */	u32							m_current_frame_id;
	/* 0x00b4 */	bool						m_process_next_frame_commands;

public:
	inline										~one_way_render_channel( ) { }
}; // class one_way_render_channel

STATIC_SIZE_ASSERT( one_way_render_channel, 0xb8 );
STATIC_SIZE_ASSERT( one_way_render_channel::null_render_command, 0x54 );

} // namespace render
} // namespace vostok

#include <vostok/render/facade/one_way_render_channel_inline.h>

#endif // VOSTOK_RENDER_ONE_WAY_RENDER_CHANNEL_H_INCLUDED
