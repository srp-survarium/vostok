////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_WORLD_H_INCLUDED
#define RENDER_WORLD_H_INCLUDED

/* INCLUDES */
class vostok::render::editor::renderer;
class vostok::render::engine::renderer;
class vostok::render::engine::world;
class vostok::render::game::renderer;
class vostok::render::one_way_render_channel;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::memory::base_allocator;

namespace vostok {
namespace render {

class world {
public:
										world						(
											memory::base_allocator&					logic_allocator,
											memory::base_allocator*					editor_allocator,
											configs::binary_config_ptr const&		in_config,
											bool									is_editor
										);
										~world						( );

			void						clear_resources				( );

			void						tick						( );

			void						enable_logic_thread_safety	( bool value );

	inline	engine::world&				engine_world				( ) { /* no source */ }
			engine::renderer&			engine_renderer				( );
			game::renderer&				game_renderer				( );
	inline	editor::renderer&			editor_renderer				( ) { /* no source */ }

	inline	one_way_render_channel&		logic_channel				( ) { /* no source */ }

			void						end_frame_logic				( );

	inline	one_way_render_channel&		editor_channel				( ) { /* no source */ }

	inline	void						end_frame_editor			( ) { /* no source */ }
private:
			void						end_frame					( );

	/* 0x0000 */	one_way_render_channel		m_logic_channel;
	/* 0x00b8 */	one_way_render_channel		m_editor_channel;
	/* 0x0170 */	engine::world*				m_render_engine_world;
	/* 0x0174 */	engine::renderer*			m_engine_renderer;
	/* 0x0178 */	game::renderer*				m_game_renderer;
	/* 0x017c */	editor::renderer*			m_editor_renderer;
	/* 0x0180 */	volatile long				m_is_logic_enabled;
	/* 0x0184 */	volatile long				m_is_logic_frame_ended;
	/* 0x0188 */	bool						m_is_editor_frame_ended;
	/* 0x0189 */	bool						m_is_editor;
}; // class world

STATIC_SIZE_ASSERT(world, 0x190);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_WORLD_H_INCLUDED
