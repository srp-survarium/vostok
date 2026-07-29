////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_MANAGER_H_INCLUDED
#define RENDER_SCENE_MANAGER_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::scene_manager>;
class vostok::render::vector<vostok::render::render_output_window *>;
class vostok::render::vector<vostok::render::scene *>;
class vostok::render::vector<vostok::render::scene_view *>;

/* FORWARD REFS */
class vostok::render::output_window_configuration;
class vostok::render::render_output_window;
class vostok::render::scene;
class vostok::render::scene_configuration;
class vostok::render::scene_view;

namespace vostok {
namespace render {

class scene_manager : public quasi_singleton< scene_manager > {
public:
										scene_manager				( );
										~scene_manager				( );

			void						add_scene					( scene* in_scene );
			void						remove_scene				( scene* in_scene );

			void						add_scene_view				( scene_view* in_scene_view );
			void						remove_scene_view			( scene_view* in_scene_view );

			void						add_render_output_window	( render_output_window* in_output_window );
	inline	void						remove_render_output_window	( render_output_window* arg_0 ) { /* no source */ }

	inline	scene*						create_scene				( scene_configuration const& arg_0 ) { /* no source */ }

	inline	void						destroy						( scene* arg_0 ) { /* no source */ }
	inline	void						destroy						( scene_view* arg_0 ) { /* no source */ }
	inline	void						destroy						( render_output_window* arg_0 ) { /* no source */ }

	inline	scene_view*					create_scene_view			( ) { /* no source */ }
	inline	render_output_window*		create_render_output_window	( output_window_configuration const& arg_0 ) { /* no source */ }

	inline	u32							scene_count					( ) const { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< scene_manager > */
private:
	/* 0x0000 */	vector< scene* >					m_scenes;
	/* 0x000c */	vector< scene_view* >				m_views;
	/* 0x0018 */	vector< render_output_window* >		m_output_windows;
}; // class scene_manager

STATIC_SIZE_ASSERT(scene_manager, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_MANAGER_H_INCLUDED
