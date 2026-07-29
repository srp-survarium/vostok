////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_FACADE_GAME_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_GAME_RENDERER_H_INCLUDED

#include <vostok/render/api.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/world.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include <vostok/scaleform/sources/scaleform_render_command.h>

namespace survarium {
struct flash_text_manager;
}

namespace vostok {

namespace ui {
struct font;
}

namespace render {

class scene_renderer;
class world;

namespace debug {
class renderer;
}

namespace ui {
class renderer;
}

namespace game {

class VOSTOK_RENDER_API renderer : public core::noncopyable {
private:
	friend class render::world;
	renderer	( render::world& world, engine::world& engine_world );

public:
	~renderer	( );

	debug::renderer&	debug	( ) const;
	ui::renderer&		ui		( ) const;
	scene_renderer&		scene	( ) const;

	inline engine::world& engine_world( )
	{
		return m_render_engine_world;
	}

	inline void resize_render_output_window(
		base_output_window_ptr const&	output_window,
		u32 const						width,
		u32 const						height,
		bool const						fullscreen
	)
	{
		m_render_engine_world.resize_render_output_window(
			output_window,
			width,
			height,
			fullscreen
		);
	}

	void	goto_fullscreen	( base_output_window_ptr const& output_window );

	void	show_movie			(
				base_scene_view_ptr const&			scene_view,
				survarium::flash_movie_resource_ptr	movie
			);
	void	hide_movie			(
				base_scene_view_ptr const&			scene_view,
				survarium::flash_movie_resource_ptr	movie
			);

	void	show_text_manager	(
				base_scene_view_ptr const&	scene_view,
				survarium::flash_text_manager*	tm
			);
	void	hide_text_manager	(
				base_scene_view_ptr const&	scene_view,
				survarium::flash_text_manager*	tm
			);

	void	execute_scaleform_command	( survarium::scaleform_render_command command );

	void	draw_scene	(
				base_scene_ptr const&			scene,
				base_scene_view_ptr const&		scene_view,
				base_output_window_ptr const&	render_output_window,
				math::rectangle< float2 > const&	viewport,
				vostok::ui::font const*			default_font
			);
	void	end_frame	( );

private:
	struct draw_scene_params;
	void	draw_scene_impl	( draw_scene_params const& params );

private:
	/* 0x0000 */	render::world&		m_world;
	/* 0x0004 */	engine::world&		m_render_engine_world;
	/* 0x0008 */	debug::renderer*	m_debug;
	/* 0x000c */	ui::renderer*		m_ui;
	/* 0x0010 */	scene_renderer*		m_scene;
}; // class renderer

STATIC_SIZE_ASSERT( renderer, 0x14 );

} // namespace game
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_FACADE_GAME_RENDERER_H_INCLUDED
