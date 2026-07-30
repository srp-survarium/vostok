#ifndef VOSTOK_RENDER_ENGINE_SCENE_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_MANAGER_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>

namespace vostok {
namespace render {

struct output_window_configuration;
struct scene_configuration;

class render_output_window;
class scene;
class scene_view;

class scene_manager : public quasi_singleton< scene_manager > {
public:
	scene_manager( );
	~scene_manager( );

	void add_scene( scene* in_scene );
	void remove_scene( scene* in_scene );

	void add_scene_view( scene_view* in_scene_view );
	void remove_scene_view( scene_view* in_scene_view );

	void add_render_output_window( render_output_window* in_output_window );
	void remove_render_output_window( render_output_window* in_output_window );

	scene* create_scene( scene_configuration const& renderer_configuration );
	void destroy( scene* scene );

	scene_view* create_scene_view( );
	void destroy( scene_view* view );

	render_output_window* create_render_output_window(
		output_window_configuration const& window_configuration
	);
	void destroy( render_output_window* output_window );

	u32 scene_count( ) const { return m_scenes.size( ); }

private:
	vector< scene* > m_scenes;
	vector< scene_view* > m_views;
	vector< render_output_window* > m_output_windows;
};

STATIC_SIZE_ASSERT( scene_manager, 0x24 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_MANAGER_H_INCLUDED
