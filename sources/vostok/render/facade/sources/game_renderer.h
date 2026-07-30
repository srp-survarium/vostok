#ifndef VOSTOK_RENDER_FACADE_SOURCES_GAME_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_GAME_RENDERER_H_INCLUDED

#include <vostok/math_rectangle.h>
#include <vostok/render/api.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/engine/world.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include <vostok/scaleform/sources/scaleform_render_command.h>

namespace survarium {
struct flash_text_manager;
}

namespace vostok {

namespace ui { struct font; }

namespace render {

class scene_renderer;
class world;
namespace debug { class renderer; }
namespace ui { class renderer; }

namespace game {

class renderer : public core::noncopyable {
private:
	friend class render::world;
	renderer( render::world& world, engine::world& engine_world );

public:
	~renderer( );

	debug::renderer& debug( ) const;
	ui::renderer& ui( ) const;
	scene_renderer& scene( ) const;

	engine::world& engine_world( ) { return m_render_engine_world; }

	void resize_render_output_window(
		base_output_window_ptr const& output_window,
		u32 width,
		u32 height,
		bool fullscreen
	)
	{
		m_render_engine_world.resize_render_output_window(
			output_window,
			width,
			height,
			fullscreen
		);
	}

	void goto_fullscreen( base_output_window_ptr const& output_window );
	void show_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie );
	void hide_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie );
	void show_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* manager );
	void hide_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* manager );
	void execute_scaleform_command( survarium::scaleform_render_command command );

	void draw_scene(
		base_scene_ptr const& scene,
		base_scene_view_ptr const& scene_view,
		base_output_window_ptr const& render_output_window,
		math::rectangle< float2 > const& viewport,
		vostok::ui::font const* default_font
	);
	void end_frame( );

private:
	struct draw_scene_params {
		draw_scene_params( ) { }
		~draw_scene_params( ) { }

		base_scene_ptr scene;
		base_scene_view_ptr scene_view;
		base_output_window_ptr render_output_window;
		math::rectangle< float2 > viewport;
		vostok::ui::font const* default_font;
	};

	void draw_scene_impl( draw_scene_params const& params );

private:
	render::world& m_world;
	engine::world& m_render_engine_world;
	debug::renderer* m_debug;
	ui::renderer* m_ui;
	scene_renderer* m_scene;
};

} // namespace game
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_GAME_RENDERER_H_INCLUDED
