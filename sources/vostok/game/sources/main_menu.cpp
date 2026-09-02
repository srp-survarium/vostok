// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "main_menu.h"
#include "main_menu_external_handler.h"

#include <vostok/input/world.h>
#include <vostok/resources.h>
#include <vostok/render/facade/common_types.h>

#include "game.h"

namespace survarium {

main_menu::main_menu( game& g )
	: base_game_scene( g )
{
	query_resources( );
}


main_menu::~main_menu( )
{
}

void main_menu::on_deactivate( )
{
	base_game_scene::on_deactivate( );
	get_game( ).input_world( ).remove_handler( *this );
}

void main_menu::clear_resources( )
{
}

void main_menu::on_activate( )
{
	base_game_scene::on_activate( );
	get_game( ).input_world( ).add_handler( *this );
}

void main_menu::tick(
	const u32		frame_delta_in_ms,
	const u32		current_time_in_ms,
	const bool		is_game_paused
)
{
	base_game_scene::tick( frame_delta_in_ms, current_time_in_ms, is_game_paused );
}

void main_menu::query_resources( )
{
	render::scene_configuration				render_configuration;
	render_configuration.m_create_terrain			= false;
	render_configuration.m_create_particle_world	= false;

	resources::user_data_variant temp_data;
	temp_data.set( render_configuration );

	resources::user_data_variant const* data[] = { &temp_data, 0, 0 };

	resources::request requests[] =
	{
		{ "game_scene", resources::scene_class },
		{ "game_scene_view", resources::scene_view_class },
	};
	resources::query_resources(
		requests,
		boost::bind( &main_menu::on_resources_ready, this, _1 ),
		g_allocator,
		data
	);
}

void main_menu::on_resources_ready( resources::queries_result& data )
{
	m_render_scene		= static_cast_resource_ptr< render::scene_ptr >( data[0].get_unmanaged_resource( ) );
	m_render_scene_view	= static_cast_resource_ptr< render::scene_view_ptr >( data[1].get_unmanaged_resource( ) );
}

} // namespace survarium
