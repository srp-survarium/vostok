////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the survarium network-client
// carcass (network_client / base_network_client / lobby_client / match_client /
// messaging_client / network_stats stats_row+stats_stream). These live in the
// game module, so they can NOT be anchored from game_core's anchor_network.cpp
// (game_core must not reference up into game). Instead game_world's ctor calls
// anchor_game_network_clients( *this->m_game ), which references every out-of-line
// client symbol so the linker keeps the carcass objects in the base EXE for the
// delinker to score (a stub body references nothing on its own).
//
// Retire once the real game call graph (game::create/set_network_client, the
// matched-target owner of m_network_client) reaches these - that body-matching +
// anchor-retirement is a later phase.

#include "pch.h"

#include <vostok/network/world.h>
#include <vostok/network_core/udp_match_stats.h>

#include "network_client.h"
#include "base_network_client.h"
#include "lobby_client.h"
#include "match_client.h"
#include "messaging_client.h"
#include "network_stats.h"
#include "flash_text_manager.h"
#include "game.h"
#include "player.h"	// network_client holds player_ptr members

namespace survarium {

void anchor_game_network_clients( game& g )
{
	// the client ctors bind member sub-objects to placeholder world refs (the
	// stub bodies never deref them), and the network_client object is ~16KB - so
	// guard the heavy construction behind a never-true flag: the compiler still
	// emits every reference (/OPT:REF keeps the symbols) but it never runs.
	static volatile bool s_run = false;
	if( !s_run )
		return;

	network::world& world = *( network::world* )NULL;

	// a runtime (non-constant) player id: the anchor is the only caller of the
	// matched out-of-line bodies, so a literal arg lets LTCG constant-propagate it
	// into them (e.g. is_player_current( 0 ) specialises `id` to 0 and drops the
	// stack param). Reading the volatile flag yields a value the compiler can't fold.
	const u8 runtime_id = (u8)s_run;

	// being the sole caller also lets LTCG give the matched bodies a custom register
	// calling convention (param in a reg, no `ret 4`) instead of the standard __thiscall
	// the real game emits. Take the member-function addresses through a volatile sink so
	// the linker must keep them callable via the normal ABI, pinning the convention.
	{
		bool ( base_network_client::* const is_cur )( const u8 ) const = &base_network_client::is_player_current;
		game_team_id ( network_client::* const team )( pcstr ) = &network_client::get_player_team;
		static pcvoid volatile s_sink = 0;
		s_sink = *( pcvoid const* )&is_cur;
		s_sink = *( pcvoid const* )&team;
	}

	{
		// network_client transitively constructs base_network_client, lobby_client,
		// match_client, messaging_client and the network_stats stats_row members,
		// and emits its own vtable + the pure-virtual overrides.
		network_client client( g, false );

		base_network_client& base = client;
		base.get_current_player_position( );
		base.fill_current_player_stats( boost::function< void( u32, float, float, pcstr ) >( ) );
		base.is_player_current( runtime_id );
		base.current_player_team( );
		base.get_current_player( );
		base.use_physics_controller_for_current( "" );
		base.attach_to_player( player_ptr( ) );
		base.detach_from_player( );

		client.connect_to_login( "host", 0, "account", "password" );
		client.disconnect( );
		client.tick( 0, false );
		client.send_local_player_input( *( player_input* )NULL, 0, float4x4( ), 0.f );
		client.initiate_kill_current_player( );
		client.is_player_local( runtime_id );
		client.unload( );
		client.get_player( runtime_id );
		client.get_player_team( "name" );
	}

	{
		lobby_client lobby( g );
		lobby.session_id( );
		lobby.connect( *( server_connection_info* )NULL );
		lobby.disconnect( );
		lobby.ping_server( );
	}

	{
		messaging_client messaging( g );
		messaging.connect( *( server_connection_info* )NULL );
		messaging.disconnect( );
		messaging.query_for_friend_list( );
		messaging.find_players_by_name( "name" );
	}

	{
		match_client match( world );
		match.disconnect( );
		match.send_queued_packets( 0 );
	}

	{
		// the stats_row / stats_stream out-of-line bodies live in network_stats.cpp;
		// call them explicitly so the linker keeps them (members alone don't).
		flash_text_manager& text_manager = *( flash_text_manager* )NULL;
		network_core::udp_match_stream_stats stream_stats;
		network_core::udp_match_items_stats items_stats;

		stats_stream stream;
		stream.create( text_manager, 0.f, 0.f, 0.f, 0.f, 0.f, math::color( ) );
		stream.set_text( 0, items_stats, items_stats );

		stats_row row;
		row.create( text_manager, "caption", 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, math::color( ) );
		row.set_text( 0, stream_stats, stream_stats );
	}
}

} // namespace survarium
