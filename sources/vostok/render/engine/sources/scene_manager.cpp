#include "pch.h"
#include <vostok/render/facade/common_types.h>
#include "render_output_window.h"
#include "scene.h"
#include "scene_manager.h"
#include "scene_view.h"

namespace vostok {
namespace render {

struct render_output_window_predicate {
	explicit render_output_window_predicate( HWND__* window ) :
		m_window( window )
	{
	}

	bool operator()( render_output_window* other )
	{
		return m_window == other->window( );
	}

	HWND__* m_window;
};

// claude@NOTE: no legacy ancestor - legacy scene_manager declared no ctor (compiler-generated); the explicit ctor is new-in-target; matcher-phase work.
// STATE[STUB]
 scene_manager::scene_manager( )
{
	// FUNCTION BODY[0x749240]: 1
	// <0>
	// ******
}

 scene_manager::~scene_manager( )
{
	// FUNCTION BODY[0x749270]
	vector< scene* >::iterator	it_c = m_scenes.begin(),
								en_c = m_scenes.end();

//!	ASSERT( it_c == en_c, "Some scenes were not deleted before render engine destruction." );
	if ( it_c != en_c )
		LOG_ERROR		( "Some scenes were not deleted before render engine destruction." );

	for( ; it_c != en_c; ++it_c)
		DELETE			(*it_c);

	vector< scene_view* >::iterator	it_v = m_views.begin(),
									en_v = m_views.end();

	ASSERT( it_v == en_v, "Some scene views were not deleted before render engine destruction." );

	for( ; it_v != en_v; ++it_v)
		DELETE(*it_v);
}

void scene_manager::add_scene( scene* in_scene )
{
	// FUNCTION BODY[0x749210]
	m_scenes.push_back( in_scene);
}

void scene_manager::remove_scene( scene* in_scene )
{
	// FUNCTION BODY[0x749170]
	vector< scene* >::iterator found = std::find(m_scenes.begin(), m_scenes.end(), in_scene);

	if (found == m_scenes.end())
		return;

	m_scenes.erase(found);
}

void scene_manager::add_scene_view( scene_view* in_scene_view )
{
	// FUNCTION BODY[0x7491e0]
	m_views.push_back(in_scene_view);
}

void scene_manager::remove_scene_view( scene_view* in_scene_view )
{
	// FUNCTION BODY[0x749120]
	vector< scene_view* >::iterator found = std::find(m_views.begin(), m_views.end(), in_scene_view);

	if(found == m_views.end())
		return;

	m_views.erase(found);
}

void scene_manager::add_render_output_window( render_output_window* in_output_window )
{
	// FUNCTION BODY[0x7491b0]
	m_output_windows.push_back(in_output_window);
}

// claude@NOTE: the seven definitions below were declared by the canonical header but
// left undefined by the carcass (no target symbol - /OPT:REF stripped them); ported
// from the legacy scene_manager.cpp, with its scenes/scene_views/render_output_windows
// typedefs resolved to the canonical direct vector< T* > members.
void scene_manager::remove_render_output_window( render_output_window* in_output_window )
{
	vector< render_output_window* >::iterator found = std::find(m_output_windows.begin(), m_output_windows.end(), in_output_window);

	if(found == m_output_windows.end())
		return;

	m_output_windows.erase(found);
}

scene* scene_manager::create_scene( scene_configuration const& renderer_configuration )
{
	scene* new_scene	= NEW(scene) (renderer_configuration);
	m_scenes.push_back(new_scene);

	return new_scene;
}

void scene_manager::destroy( scene* scene )
{
	vector< render::scene* >::iterator it = std::find( m_scenes.begin(), m_scenes.end(), scene);

	ASSERT( it != m_scenes.end());
	if( it == m_scenes.end())
		return;

	DELETE			(*it);
	m_scenes.erase	(it);
}

scene_view* scene_manager::create_scene_view( )
{
	scene_view* new_view	= NEW(scene_view);
	m_views.push_back(new_view);

	return new_view;
}

void scene_manager::destroy( scene_view* view )
{
	vector< scene_view* >::iterator it = std::find( m_views.begin(), m_views.end(), view);

	ASSERT( it != m_views.end());
	if( it == m_views.end())
		return;

	DELETE			(*it);
	m_views.erase	(it);
}

render_output_window* scene_manager::create_render_output_window(
	output_window_configuration const& window_configuration
)
{
	HWND__* window = (HWND__*)window_configuration.hwnd;
 	vector< render_output_window* >::iterator it = std::find_if( m_output_windows.begin(), m_output_windows.end(), render_output_window_predicate( window) );
 	if( it != m_output_windows.end())
	{
		ASSERT( 0, "A window output with the specified handle has been crate already.");
 		return *it;
	}

	render_output_window* new_output	= NEW(render_output_window)( window_configuration );
	m_output_windows.push_back			( new_output );

	return new_output;
}

void scene_manager::destroy( render_output_window* output_window )
{
	vector< render_output_window* >::iterator const found	= std::find( m_output_windows.begin(), m_output_windows.end(), output_window );
	R_ASSERT									( found != m_output_windows.end() );
	DELETE										( *found );
	m_output_windows.erase						( found );
	R_ASSERT									( std::find( m_output_windows.begin(), m_output_windows.end(), output_window ) == m_output_windows.end() );
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
