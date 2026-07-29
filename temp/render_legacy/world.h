////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_WORLD_H_INCLUDED
#define VOSTOK_RENDER_WORLD_H_INCLUDED

#include <vostok/render/api.h>
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/one_way_render_channel.h>

namespace vostok {
namespace render {

namespace editor	{ class renderer; }
namespace engine	{ class renderer; class world; }
namespace game		{ class renderer; }

class VOSTOK_RENDER_API world {
public:
									world						(
										memory::base_allocator&				logic_allocator,
										memory::base_allocator*				editor_allocator,
										configs::binary_config_ptr const&	in_config,
										bool								is_editor
									);
									~world						( );

			void					clear_resources				( );
			void					tick						( );
			void					enable_logic_thread_safety	( bool value );

	inline	engine::world&			engine_world				( ) { return *m_render_engine_world; }
			engine::renderer&		engine_renderer				( );
			game::renderer&			game_renderer				( );
	inline	editor::renderer&		editor_renderer				( ) { return *m_editor_renderer; }

	inline	one_way_render_channel&	logic_channel				( ) { return m_logic_channel; }
			void					end_frame_logic				( );

	inline	one_way_render_channel&	editor_channel				( ) { return m_editor_channel; }
	inline	void					end_frame_editor			( )
	{
#ifndef MASTER_GOLD
		m_editor_channel.render_on_end_frame( );
#endif
		R_ASSERT					( !m_is_editor_frame_ended );
		m_is_editor_frame_ended		= true;

		if ( m_is_logic_frame_ended )
			end_frame				( );
	}

private:
									world						( world const& );
			world&					operator =					( world const& );
			void					end_frame					( );

private:
	/* 0x0000 */	one_way_render_channel	m_logic_channel;
	/* 0x00b8 */	one_way_render_channel	m_editor_channel;
	/* 0x0170 */	engine::world*			m_render_engine_world;
	/* 0x0174 */	engine::renderer*		m_engine_renderer;
	/* 0x0178 */	game::renderer*			m_game_renderer;
	/* 0x017c */	editor::renderer*		m_editor_renderer;
	/* 0x0180 */	volatile long			m_is_logic_enabled;
	/* 0x0184 */	volatile long			m_is_logic_frame_ended;
	/* 0x0188 */	bool					m_is_editor_frame_ended;
	/* 0x0189 */	bool					m_is_editor;
}; // class world

STATIC_SIZE_ASSERT( world, 0x190 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_WORLD_H_INCLUDED
