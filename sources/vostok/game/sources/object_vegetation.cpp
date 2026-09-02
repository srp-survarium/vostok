// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "object_vegetation.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/engine/sources/grass_data.h>

namespace survarium {

object_vegetation::object_vegetation( base_game_scene& s ) :
	game_object_( s )
{
}

void object_vegetation::on_grass_loaded( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	m_grass = data[ 0 ].get_unmanaged_resource( );
	cb( *this );
}

void object_vegetation::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	render::grass_loading_data* grass_data = VOSTOK_NEW_IMPL( g_allocator, render::grass_loading_data );
	grass_data->t_current = &t;
	grass_data->project_resources_path = project_resources_path;

	resources::user_data_variant ud;
	ud.set( grass_data );

	resources::query_resource(
		"grass",
		resources::grass_world_class,
		boost::bind( &object_vegetation::on_grass_loaded, this, _1, cb ),
		g_allocator,
		&ud
	);
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
