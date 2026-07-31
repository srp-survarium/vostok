////////////////////////////////////////////////////////////////////////////
//	Created		: 06.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER: dtor, add_scene, remove_scene, add_scene_view,
// remove_scene_view and add_render_output_window ported to
// sources/.../scene_manager.cpp (the render_output_window_predicate struct is
// already canonical there). Remaining here: remove_render_output_window,
// create_scene, create_scene_view, create_render_output_window and the three
// destroy overloads - declared in the canonical header but not yet defined in
// the carcass. scene_manager.h was fully superseded by the canonical header
// and removed (its scenes/scene_views/render_output_windows typedefs were
// dropped canonically - members are direct vector< T* >).

#include "pch.h"
#include "scene_manager.h"
#include "scene.h"
#include "scene_view.h"
#include "render_output_window.h"
#include <vostok/render/core/render_target.h>
#include "material.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_geometry.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/facade/common_types.h>

namespace vostok {
namespace render {

void scene_manager::remove_render_output_window	( render_output_window* in_output_window)
{
	render_output_windows::iterator found = std::find(m_output_windows.begin(), m_output_windows.end(), in_output_window);

	if(found == m_output_windows.end())
		return;

	m_output_windows.erase(found);
}

scene*	scene_manager::create_scene( vostok::render::scene_configuration const& renderer_configuration)
{
	scene* new_scene	= NEW(scene) (renderer_configuration);
	m_scenes.push_back(new_scene);

	return new_scene;
}

void	scene_manager::destroy( scene* _scene)
{
	scenes::iterator it = std::find( m_scenes.begin(), m_scenes.end(), _scene);

	ASSERT( it != m_scenes.end());
	if( it == m_scenes.end())
		return;

	DELETE			(*it);
	m_scenes.erase	(it);
}

scene_view*	scene_manager::create_scene_view()
{
	scene_view* new_view	= NEW(scene_view);
	m_views.push_back(new_view);

	return new_view;
}

void	scene_manager::destroy( scene_view* view)
{
	scene_views::iterator it = std::find( m_views.begin(), m_views.end(), view);

	ASSERT( it != m_views.end());
	if( it == m_views.end())
		return;

	DELETE			(*it);
	m_views.erase	(it);
}

struct render_output_window_predicate
{
	render_output_window_predicate ( HWND window) : m_window(window){}

	bool operator ()( render_output_window * other)
	{
		return m_window == other->window();
	}

	HWND		m_window;
};

render_output_window*	scene_manager::create_render_output_window( vostok::render::output_window_configuration const& window_configuration )
{
	HWND window = (HWND)window_configuration.hwnd;
 	render_output_windows::iterator it = std::find_if( m_output_windows.begin(), m_output_windows.end(), render_output_window_predicate( window) );
 	if( it != m_output_windows.end())
	{
		ASSERT( 0, "A window output with the specified handle has been crate already.");
 		return *it;
	}

	render_output_window* new_output	= NEW(render_output_window)( window_configuration );
	m_output_windows.push_back			( new_output );

	return new_output;
}

void scene_manager::destroy( render_output_window* output_window)
{
	render_output_windows::iterator const found	= std::find( m_output_windows.begin(), m_output_windows.end(), output_window );
	R_ASSERT									( found != m_output_windows.end() );
	DELETE										( *found );
	m_output_windows.erase						( found );
	R_ASSERT									( std::find( m_output_windows.begin(), m_output_windows.end(), output_window ) == m_output_windows.end() );
}

} // namespace render
} // namespace vostok
