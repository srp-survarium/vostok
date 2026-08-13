////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "base_network_client.h"

// resource_ptr<player> member (m_current_player) instantiates its dtor here, so
// the complete player type is required (the header only forward-declares it)
#include "player.h"
#include "game.h"					// m_game.get_game_world()
#include "game_world.h"				// game_world::game_ui member
#include "player_input_handler.h"	// NEW( player_input_handler )( game_world& )
#include "stats_graph.h"			// NEW( stats_graph )( ... )
#include "game_memory.h"			// NEW / DELETE ( survarium::g_allocator )
#include <vostok/console_command.h>	// console_commands::cc_delegate
#include <vostok/math_constants.h>	// math::infinity

namespace survarium {

// claude@NOTE: ctor member inits (player_input_handler + the two stats_graph NEWs with
// 1.f/infinity/4.f/4.5f and 1.f/infinity/5.f/10.f, color 0xff00ff00) are byte-identical to
// the target; the residual is line-table attribution (the optimizer merges the init-list
// into one statement) plus the standalone-vftable store the anchor's network_client(g,false)
// instantiation forces (the target emits the base ctor inlined-only). Both lift when the real
// game owner constructs the client and the anchor is removed.
base_network_client::base_network_client( game& game ) :
	m_current_player			( ),
	m_input_handler				( NEW( player_input_handler )( game.get_game_world( ) ) ),
	m_linear_speed_graph		( NEW( stats_graph )( 1.f, math::infinity, 4.f, 4.5f, 0xff00ff00 ) ),
	m_angular_speed_graph		( NEW( stats_graph )( 1.f, math::infinity, 5.f, 10.f, 0xff00ff00 ) ),
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
	DELETE( m_input_handler );
	DELETE( m_linear_speed_graph );
	DELETE( m_angular_speed_graph );
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

// claude@NOTE: structure matches the target (ternary, shared float3 copy/merge). Residual
// is player.h's inline position() body: the carcass returns m_last_frame_position (0x10ec4)
// but the target reads the member at player+0x87A0 and devirtualises+inlines it; the carcass
// stub instead emits a virtual call. Reaches 100% once player.h's position() is the real body.
float3 base_network_client::get_current_player_position( ) const
{
	return m_current_player ? m_current_player->position( ) : float3( 0.f, 0.f, 0.f );
}

void base_network_client::fill_current_player_stats( boost::function< void( u32, float, float, pcstr ) > callback )
{
	if ( m_current_player )
		m_current_player->damage_model( )->dump_stats( callback );
}

// claude@NOTE: attach_to_player_cc / attach_to_player / detach_from_player statement
// structure matches the target. The remaining byte residual includes the resource_ptr
// copy paths.
void base_network_client::attach_to_player_cc( pcstr const arguments )
{
	u8 player_id;
	if ( sscanf_s( arguments, "%d", &player_id ) == -1 )
		return;

	if ( player_id >= 20 )
		return;

	player_ptr const player = get_player( player_id );
	if ( player )
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

// claude@NOTE: source shape is exhausted. The target's inlined
// on_detached_from_player passes the second false on the stack while the base uses AL,
// matching the out-of-line callee's LTCG convention gap. Reopen only with a real
// compiler-context/call-convention lever.
void base_network_client::detach_from_player( )
{
	if ( m_current_player )
		m_current_player->detach_controller( );

	m_current_player = NULL;

	m_game.get_game_world( ).game_ui.on_detached_from_player( );
}

// claude@NOTE: statement structure matches the target. Residual: the compiler hoists the two
// apply_use_physics_controller_for_current() arms to the function tail (rarely-taken branch
// layout) and the LOG_WARNING boost::function temporaries are destructed in a different order
// than the carcass's single-caller build produces - both are layout/LTCG artifacts, not source.
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
		LOG_WARNING( "invalid argument has been passed" );
		LOG_WARNING( "expected: 1/0, on/off, true/false" );
	}
}

} // namespace survarium
