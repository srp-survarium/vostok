////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ONE_WAY_RENDER_CHANNEL_H_INCLUDED
#define RENDER_ONE_WAY_RENDER_CHANNEL_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::one_way_threads_channel<vostok::intrusive_spsc_queue<vostok::render::base_command,vostok::render::base_command,4>,vostok::intrusive_spsc_queue<vostok::render::base_command,vostok::render::base_command,4> >;
class vostok::threading::event;
typedef vostok::intrusive_list<vostok::render::base_command,vostok::render::base_command *,8,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>
	vostok::render::base_command_list;
typedef vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_view_ptr;
class vostok::render::base_command;
class vostok::render::base_scene;
class vostok::render::base_scene_view;

/* FORWARD REFS */
class vostok::memory::base_allocator;

namespace vostok {
namespace render {

class one_way_render_channel : public core::noncopyable {
public:
	inline	explicit	one_way_render_channel		( memory::base_allocator& owner_allocator ) { /* no source */ }

	inline	void		owner_initialize			( ) { /* no source */ }
	inline	void		owner_finalize				( ) { /* no source */ }
	inline	void		owner_push_back				( base_command* arg_0 ) { /* no source */ }
	inline	void		owner_delete_processed_items( ) { /* no source */ }

			bool		render_process_commands		( bool wait_for_command_if_queue_is_empty );
			void		render_on_draw_scene		(
							base_scene_ptr const&			scene,
							base_scene_view_ptr const&		scene_view,
							bool							use_depth
						);
	inline	void		render_on_draw_scene2		( base_scene_ptr* arg_0, base_scene_view_ptr* arg_1, bool arg_2 ) { /* no source */ }
			void		render_on_end_frame			( );
private:
	inline	void		render_initialize			( ) { /* no source */ }
	inline	bool		render_is_queue_empty		( ) const { /* no source */ }

			void		process_next_frame_commands	( );

public:
	inline				~one_way_render_channel		( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	one_way_threads_channel< intrusive_spsc_queue< base_command, base_command, 4 >, intrusive_spsc_queue< base_command, base_command, 4 > >	m_channel;
	/* 0x0090 */	threading::event		m_wait_form_command_event;
	/* 0x0098 */	base_command_list		m_next_frame_commands_queue;
	/* 0x00a8 */	base_scene_ptr			m_scenes;
	/* 0x00ac */	base_scene_view_ptr		m_scene_views;
	/* 0x00b0 */	u32						m_current_frame_id;
	/* 0x00b4 */	bool					m_process_next_frame_commands;
}; // class one_way_render_channel

STATIC_SIZE_ASSERT(one_way_render_channel, 0xB8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ONE_WAY_RENDER_CHANNEL_H_INCLUDED
