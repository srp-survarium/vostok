////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_vegetation.h"

namespace survarium {

// claude@NOTE: load / insert / remove / on_grass_loaded are walled by render-facade
// infra absent from our source tree: load needs render::grass_loading_data +
// resources::query_resources; insert/remove call render::scene_renderer::set_grass /
// reset_grass; on_grass_loaded builds an intrusive_ptr from a render::grass_loading_data
// member of queries_result. None of grass_loading_data / set_grass / reset_grass /
// query_resources are declared in our render facade (vostok/render/facade) - they
// belong to the still-unported grass cook. Those four stay STUB until that lands.
// The ctor is facade-free and recovered/anchored below.
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

// STATE[STUB]
// target: 1 stmt - m_game_scene...get_scene_renderer().set_grass( m_grass );
// Walled by render::scene_renderer::set_grass.
void object_vegetation::insert( )
{
}

// STATE[STUB]
// target: 1 stmt - m_game_scene...get_scene_renderer().reset_grass( m_grass );
// Walled by render::scene_renderer::reset_grass.
void object_vegetation::remove( )
{
}

} // namespace survarium
