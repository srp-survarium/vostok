////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "base_network_client.h"

// resource_ptr<player> member (m_current_player) instantiates its dtor here, so
// the complete player type is required (the header only forward-declares it)
#include "player.h"

namespace survarium {

// claude@NOTE: ctor/dtor and attach_to_player/detach_from_player are PARKED on missing
// game-module carcass: the ctor NEWs a player_input_handler (0x1A4 bytes, push 1A4h) whose
// type is not in the carcass sources (only canonical headers/player_input_handler.h), and it
// passes m_game.get_game_world() (an inline game_world member at game+0x98) which the
// simplified carcass game.h declares as a pointer, not the canonical inline member. attach/
// detach reach m_game.get_game_world().game_ui() (a game_world_ui at game+0x26C, also absent
// from the carcass). Recoverable once player_input_handler + the canonical game/game_world/
// game_world_ui layout are pulled into the game-module carcass.
// STATE[STUB]
 base_network_client::base_network_client( game& game ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game( game )
{
	// STATICS
	// static console_commands::cc_delegate s_use_physics_controller_for_current_command = <0x4c2b3e8>;
	// ******

	// FUNCTION BODY[0x740d70]: 9
	// <0x740d70>|0x000|+0x03a:'21'
	// <0x740daa>|0x03a|+0x064:'22'
	// <0x740e0e>|0x09e|+0x05f:'23'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x740e6d>|0x0fd|+0x093:'29'
	// ******
}

// STATE[STUB]
 base_network_client::~base_network_client( )
{
	// CALL SITE INFO
	// <0x740c3a> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x740c10]: 3
	// <0x740c15>|0x005|+0x03b:'34'
	// <0x740c50>|0x040|+0x026:'35'
	// <0x740c76>|0x066|+0x026:'36'
	// ******
}

bool base_network_client::is_player_current( const u8 id ) const
{
	return m_current_player && m_current_player->id == id;
}

game_team_id base_network_client::current_player_team( ) const
{
	if ( m_current_player.c_ptr( ) )
		return m_current_player.c_ptr( )->team( );

	return team_neutral;
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

// TU static console command (compiler-generated atexit destructor); a matcher
// recovers it together with the ctor's cc_delegate static when this TU is enabled.
/*
// STATE[STUB]
void `survarium::base_network_client::base_network_client'::`2'::`dynamic atexit destructor for 's_use_physics_controller_for_current_command''( )
{
	// FUNCTION BODY[0x7f05f0]: 12
	// <0>
	// <0x741014>|0x004|+0x01e:'68'
	// <0>
	// <1>
	// <0x741032>|0x022|+0x008:'71'
	// <0>
	// <1>
	// <0x74103a>|0x02a|+0x00f:'74'
	// <0x741049>|0x039|+0x005:'75'
	// <0x74104e>|0x03e|+0x002:'76'
	// <0>
	// <0x741050>|0x040|+0x013:'78'
	// ******
}
*/

// STATE[STUB]
void base_network_client::attach_to_player( player_ptr player )
{
	// FUNCTION BODY[0x740f10]: 8
	// <0x740f14>|0x004|+0x010:'83'
	// <0x740f24>|0x014|+0x005:'84'
	// <0>
	// <0x740f29>|0x019|+0x043:'86'
	// <0>
	// <0x740f6c>|0x05c|+0x03a:'88'
	// <0>
	// <0x740fa6>|0x096|+0x01c:'90'
	// ******
}

// STATE[STUB]
void base_network_client::detach_from_player( )
{
	// FUNCTION BODY[0x740cd0]: 6
	// <0x740cda>|0x00a|+0x010:'95'
	// <0>
	// <0x740cea>|0x01a|+0x005:'97'
	// <0x740cef>|0x01f|+0x02a:'98'
	// <0x740d19>|0x049|+0x019:'99'
	// <0>
	// ******
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
