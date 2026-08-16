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

// claude@NOTE: the old park cause for load / on_grass_loaded is stale -
// render::grass_loading_data (render/engine/sources/grass_data.h) and
// resources::query_resources (resources.h) both exist now. What is still open is the
// queries_result accessor the target reads: on_grass_loaded builds the intrusive_ptr
// temp from `&data + 0x12C` (a raw pointer inside the trailing m_queries[] array, not a
// named member) before assigning m_grass, and load's variant<32>/boost::bind argument
// pack is unreconstructed.
object_vegetation::object_vegetation( base_game_scene& s ) :
	game_object_( s )
{
}

// STATE[STUB]
// target: 2 stmts - stmt 1 = m_grass = <ptr at queries_result+0x12C> (intrusive_ptr
// assign); stmt 2 = cb( *this ). See the note above the ctor.
void object_vegetation::on_grass_loaded( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
}

// STATE[STUB]
// target: malloc variant<32> ud; ud = t; ud += project_resources_path; build the
// on_grass_loaded callback via boost::bind; query_resources( "grass", ... ).
// See the note above the ctor.
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
