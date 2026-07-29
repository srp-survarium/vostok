////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GAME_RENDERER_DRAW_SCENE_PARAMS_H_INCLUDED
#define RENDER_GAME_RENDERER_DRAW_SCENE_PARAMS_H_INCLUDED

/* INCLUDES */
class vostok::math::rectangle<vostok::math::float2>;
struct vostok::ui::font;
typedef vostok::resources::resource_ptr<vostok::render::base_output_window,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_output_window_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_view_ptr;
class vostok::render::base_output_window;
class vostok::render::base_scene;
class vostok::render::base_scene_view;

namespace vostok {
namespace render {

struct game::renderer::draw_scene_params {
	inline		draw_scene_params	( ) { /* no source */ }
	inline		~draw_scene_params	( ) { /* no source */ }

	/* 0x0000 */	base_scene_ptr					scene;
	/* 0x0004 */	base_scene_view_ptr				scene_view;
	/* 0x0008 */	base_output_window_ptr			render_output_window;
	/* 0x000c */	math::rectangle< float2 >		viewport;
	/* 0x001c */	ui::font const*					default_font;
}; // struct game::renderer::draw_scene_params

STATIC_SIZE_ASSERT(game::renderer::draw_scene_params, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GAME_RENDERER_DRAW_SCENE_PARAMS_H_INCLUDED
