// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "base_network_client.h"
#include "player.h"
#include "game.h"					// m_game.get_game_world()
#include "game_world.h"				// game_world::game_ui member
#include "player_input_handler.h"	// player_input_handler( game_world& )
#include "stats_graph.h"			// stats_graph( ... )
#include "game_memory.h"			// survarium::g_allocator
#include <vostok/console_command.h>	// console_commands::cc_delegate
#include <vostok/math_constants.h>	// math::infinity

namespace survarium {

base_network_client::base_network_client( game& game ) :
	m_current_player			( ),
	m_input_handler				( VOSTOK_NEW_IMPL( ::survarium::g_allocator, player_input_handler )( game.get_game_world( ) ) ),
	m_linear_speed_graph		( VOSTOK_NEW_IMPL( ::survarium::g_allocator, stats_graph )( 1.f, math::infinity, 4.f, 4.5f, 0xff00ff00 ) ),
	m_angular_speed_graph		( VOSTOK_NEW_IMPL( ::survarium::g_allocator, stats_graph )( 1.f, math::infinity, 5.f, 10.f, 0xff00ff00 ) ),
	m_game						( game )
{
	static console_commands::cc_delegate s_use_physics_controller_for_current_command(
		"use_physics_controller_for_current",
		boost::bind( &base_network_client::use_physics_controller_for_current, this, _1 ),
		true,
		console_commands::command_type_engine_internal
	);
}

base_network_client::~base_network_client( )
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_input_handler );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_linear_speed_graph );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_angular_speed_graph );
}

bool base_network_client::is_player_current( const u8 id ) const
{
	return m_current_player && m_current_player->id == id;
}

game_team_id base_network_client::current_player_team( ) const
{
	return m_current_player.c_ptr( ) ? m_current_player.c_ptr( )->team( ) : team_neutral;
}

player_ptr base_network_client::get_current_player( )
{
	return m_current_player;
}

float3 base_network_client::get_current_player_position( ) const
{
	return m_current_player.c_ptr( ) ? m_current_player->get_current( ).transform.c.xyz( ) : float3( 0.f, 0.f, 0.f );
}

void base_network_client::fill_current_player_stats( boost::function< void( u32, float, float, pcstr ) > callback )
{
	if ( m_current_player )
		m_current_player->damage_model( )->dump_stats( callback );
}

void base_network_client::attach_to_player_cc( pcstr const arguments )
{
	u8 player_id;
	if ( sscanf_s( arguments, "%d", &player_id ) == -1 )
		return;

	if ( player_id >= 20 )
		return;

	player_ptr const player = get_player( player_id );
	if ( !player )
		return;

	attach_to_player( player );
}

void base_network_client::attach_to_player( player_ptr player )
{
	if ( m_current_player )
		m_current_player->detach_controller( );

	m_current_player = player;

	m_game.get_game_world( ).game_ui.on_attached_to_player( m_current_player );

	player->attach_controller(
		m_input_handler,
		m_linear_speed_graph,
		m_angular_speed_graph,
		&m_game.get_game_world( ).game_ui
	);
}

// Matching wall: the inlined UI call passes its second false through a different LTCG convention.
void base_network_client::detach_from_player( )
{
	if ( m_current_player )
		m_current_player->detach_controller( );

	m_current_player = NULL;

	m_game.get_game_world( ).game_ui.on_detached_from_player( );
}


void base_network_client::use_physics_controller_for_current( pcstr const arguments )
{
	if ( !_stricmp( arguments, "0" ) || !_stricmp( arguments, "off" ) || !_stricmp( arguments, "false" ) )
	{
		if ( m_use_physics_controller_for_current )
		{
			m_use_physics_controller_for_current = false;
			apply_use_physics_controller_for_current( );
		}
	}
	else if ( !_stricmp( arguments, "1" ) || !_stricmp( arguments, "on" ) || !_stricmp( arguments, "true" ) )
	{
		if ( !m_use_physics_controller_for_current )
		{
			m_use_physics_controller_for_current = true;
			apply_use_physics_controller_for_current( );
		}
	}
	else
	{
		LOG_WARNING( "invalid argument has been passed to console command" );
		LOG_WARNING( "expected: 1/0, on/off, true/false" );
	}
}

// Matching wall: constructor and destructor retain only derived-owner compiler-context differences.

} // namespace survarium
