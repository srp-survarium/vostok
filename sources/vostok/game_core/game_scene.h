////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_SCENE_H_INCLUDED
#define GAME_SCENE_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

namespace survarium {

class game_scene {
public:
	virtual	void							on_activate			( ) = 0;
	virtual	void							on_deactivate		( ) = 0;
	virtual	void							tick				( u32 arg_0, u32 arg_1, bool arg_2 ) = 0;
	virtual	void							on_after_tick		( ) = 0;

	inline	render::scene_ptr const&		render_scene		( ) { return m_render_scene; }
	inline	render::scene_view_ptr const&	render_scene_view	( ) { return m_render_scene_view; }

protected:
	/* 0x0004 */	render::scene_ptr		m_render_scene;
	/* 0x0008 */	render::scene_view_ptr	m_render_scene_view;
}; // class game_scene

STATIC_SIZE_ASSERT(game_scene, 0xC);

} // namespace survarium

#endif // #ifndef GAME_SCENE_H_INCLUDED
