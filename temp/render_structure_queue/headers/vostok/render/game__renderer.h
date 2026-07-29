////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GAME_RENDERER_H_INCLUDED
#define RENDER_GAME_RENDERER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::render::debug::renderer;
class vostok::render::engine::world;
class vostok::render::scene_renderer;
class vostok::render::ui::renderer;
class vostok::render::world;
class vostok::render::base_output_window;
class vostok::render::base_scene;
class vostok::render::base_scene_view;
class survarium::flash_movie_resource;
class survarium::scaleform_render_command;

/* FORWARD REFS */
class vostok::math::rectangle<vostok::math::float2>;
class vostok::render::game::renderer::draw_scene_params;
class vostok::ui::font;
class survarium::flash_text_manager;

namespace vostok {
namespace render {

class game::renderer : public core::noncopyable {
								renderer					( world& world, engine::world& engine_world );
public:
								~renderer					( );

			debug::renderer&	debug						( ) const;

			ui::renderer&		ui							( ) const;

			scene_renderer&		scene						( ) const;

	inline	engine::world&		engine_world				( ) { /* no source */ }

	inline	void				resize_render_output_window	(
									base_output_window_ptr const&		arg_0,
									const u32							arg_1,
									const u32							arg_2,
									const bool							arg_3
								) { /* no source */ }

			void				goto_fullscreen				( base_output_window_ptr const& output_window );

			void				show_movie					( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie );
			void				hide_movie					( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie );

			void				show_text_manager			( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm );
			void				hide_text_manager			( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm );

			void				execute_scaleform_command	( survarium::scaleform_render_command command );

			void				draw_scene					(
									base_scene_ptr const&				scene,
									base_scene_view_ptr const&			scene_view,
									base_output_window_ptr const&		render_output_window,
									math::rectangle< float2 > const&	viewport,
									ui::font const*						default_font
								);

			void				end_frame					( );

private:
			void				draw_scene_impl				( game::renderer::draw_scene_params const& params );

	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	world&				m_world;
	/* 0x0004 */	engine::world&		m_render_engine_world;
	/* 0x0008 */	debug::renderer*	m_debug;
	/* 0x000c */	ui::renderer*		m_ui;
	/* 0x0010 */	scene_renderer*		m_scene;
}; // class game::renderer

STATIC_SIZE_ASSERT(game::renderer, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GAME_RENDERER_H_INCLUDED
