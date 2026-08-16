////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_vegetation.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

// claude@NOTE: load / on_grass_loaded still need render::grass_loading_data and
// resources::query_resources, neither of which our tree declares.
object_vegetation::object_vegetation( base_game_scene& s ) :
	game_object_( s )
{
}

// STATE[STUB]
// target: 2 stmts - stmt 1 = m_grass = <grass_loading_data ptr from queries_result>
// (intrusive_ptr assign); stmt 2 = cb( *this ). Walled by grass_loading_data.
void object_vegetation::on_grass_loaded( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
}

// STATE[STUB]
// target: malloc variant<32> ud; ud = t; ud += project_resources_path; build the
// on_grass_loaded callback via boost::bind; query_resources( "grass", ... ).
// Walled by render::grass_loading_data + resources::query_resources.
void object_vegetation::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
}

void object_vegetation::insert( )
{
	get_game_scene( ).renderer( ).scene( ).set_grass( m_grass, get_game_scene( ).render_scene( ) );
}

void object_vegetation::remove( )
{
	get_game_scene( ).renderer( ).scene( ).reset_grass( m_grass, get_game_scene( ).render_scene( ) );
}

} // namespace survarium
