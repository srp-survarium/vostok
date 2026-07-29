#ifndef VOSTOK_RENDER_ENGINE_SCENE_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_MANAGER_H_INCLUDED

namespace vostok {
namespace render {

class render_output_window;
class scene;
class scene_view;

class scene_manager {
public:
	scene_manager( );
	~scene_manager( );

	void add_scene( scene* in_scene );
	void remove_scene( scene* in_scene );
	void add_scene_view( scene_view* in_scene_view );
	void remove_scene_view( scene_view* in_scene_view );
	void add_render_output_window( render_output_window* in_output_window );
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_MANAGER_H_INCLUDED
