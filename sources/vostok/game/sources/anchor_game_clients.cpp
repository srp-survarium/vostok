////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the survarium network-client
// carcass (network_client / base_network_client / lobby_client / match_client /
// messaging_client / network_stats stats_row+stats_stream). These live in the
// game module, so game_core must not reference up into them. game_world's ctor calls
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
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/game_net_defines.h>
#include <vostok/resources_queries_result.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <boost/system/error_code.hpp>

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

	// volatile-sourced argument values so LTCG cannot const-propagate the anchor's
	// literals into the carcass bodies (a constant arg folds the match away)
	static volatile u32 s_u32 = 0;
	const u32 any_u32 = s_u32;

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
		static pcvoid volatile s_sink = 0;
		s_sink = *( pcvoid const* )&is_cur;
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
		// launder the string arg through a volatile so LTCG cannot const-fold the
		// literal into attach_to_player_cc (a constant "" lets it drop the sscanf_s
		// parse entirely, scoring against folded codegen instead of the real body)
		static pcstr volatile s_args = "";
		base.attach_to_player_cc( s_args );
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
		client.get_game_world( );
		client.apply_use_physics_controller_for_current( );
		client.on_connected_to_lobby( );
		client.on_disconnected_from_lobby( );
		client.on_match_disconnected( ( network_core::disconnect_event_types_enum )runtime_id );

		// private message handlers - reached only from the (still-stub) dispatch,
		// so reference them here directly (friend access) to keep them linked
		network_core::packet_reader& reader = *( network_core::packet_reader* )NULL;
		client.process_match_info( reader );
		client.process_player_profile( reader );
		client.process_team_bases( reader );
		client.query_players( );
		client.destroy_player_impl( 0 );
		client.process_player_action( reader, 0 );
		client.process_player_hit( reader );
		client.on_players_ready( *( resources::queries_result* )NULL, 0 );
		client.process_player_kill( reader );
		client.process_player_respawn( reader );
		client.process_initialize_victory_items( reader );
		client.process_base_capture_progress( reader );
		client.process_match_time( reader );
		client.process_respawn_timer( reader );
		client.process_match_wait_timer( reader );
		client.process_game_status( reader );
		client.process_player_kd_stats( reader );
		client.process_victory_item_take_or_put( reader );
		client.process_affect_damage_model( reader );
		client.player_visibility_change( reader );
		client.on_trap_placed( reader );
		client.on_trap_removed( reader );
		client.on_trap_fired( reader );
		client.on_trap_disarmed( reader );
		client.game_world_object_state_arrived( reader );
		client.on_world_sync_request( );
		client.damage_model_state_arrived( reader );
		client.send_sync_request( );
		client.process_sync_response( reader );
		client.send_player_inputs( );
		client.setup_camera_for_warmup( );

		// network-client core dispatch / lobby / http surface (still reached only
		// from stub callers, so reference directly to keep them linked for scoring)
		client.on_match_packet_received( runtime_id, reader );
		client.on_lobby_packet_received( reader );
		client.process_shop_action( reader );
		client.process_match_finished( reader );
		client.close_current_match( runtime_id != 0 );
		client.draw_stats( any_u32 );
		client.on_connected_to_match(
			( connection_error_types_enum )any_u32, ( handshaking_error_types_enum )any_u32,
			( socket_error_types_enum )any_u32, ( lobby_server_message_types_enum )any_u32 );
		client.on_connected_to_login(
			( connection_error_types_enum )any_u32, ( handshaking_error_types_enum )any_u32,
			( socket_error_types_enum )any_u32, ( login_server_message_types_enum )any_u32 );
		client.on_http_error( boost::system::error_code( ) );
		client.on_http_result_ready( "", runtime_id );
		client.http_query_server_connection_info( runtime_id );
	}

	{
		// launder the packet_reader pointer through a volatile so the compiler can
		// not see it is NULL and const-fold &reader (a NULL ref turns reader.m_*
		// field loads into absolute [4]/[8] accesses, scoring read_*() against
		// garbage codegen).
		static network_core::packet_reader* volatile reader_ptr = 0;
		network_core::packet_reader& reader = *reader_ptr;

		static server_connection_info* volatile conn_ptr = 0;
		static vector< relocate_item_descr >* volatile items_ptr = 0;
		static vectora< survarium::player_skill >* volatile skills_ptr = 0;
		static vectora< u8 >* volatile perks_ptr = 0;

		lobby_client lobby( g );
		lobby.session_id( );
		lobby.connect( *conn_ptr );
		lobby.disconnect( );
		lobby.ping_server( );

		// route every scalar arg through a volatile so the compiler can NOT const-
		// fold it into the callee (a literal 0 would make price()/query_*() etc.
		// score against folded codegen, not the real body).
		static volatile u32 v = 0;
		u32 const arg = v;

		lobby.set_status_ready_for_match( arg );
		lobby.query_client_status( ( lobby::query_info_types )arg );
		lobby.query_prices( arg );
		lobby.query_profile_contents( arg );
		lobby.discard_playing_order( );
		lobby.discard_playing_order_on_connected( );

		fixed_string< 128 > status_dest;
		lobby.status( status_dest );

		lobby.check_compatibility( arg, arg );
		lobby.can_move_item( arg, arg );
		lobby.move_item( *items_ptr );
		lobby.price( (u8)arg );
		lobby.buy_item( (u16)arg, arg, (u8)arg, arg != 0 );
		lobby.set_player_skills( *skills_ptr, *perks_ptr );
		lobby.reroll_player_skills( );

		lobby.read_status_info( reader );
		lobby.read_enumerate_profiles_info( reader );
		lobby.read_profile_content_info( reader );
		lobby.read_enumerate_inventory_info( reader );
		lobby.read_price_items( reader );
		lobby.read_profile_slots_restrictions( reader );
		lobby.read_items_compatibility( reader );
		lobby.read_account_money( reader );
		lobby.read_player_skills( reader );
		lobby.read_player_reputations( reader );
		lobby.read_player_skills_tree( reader );
		lobby.read_service_prices( reader );
		lobby.read_ping_server_answer( reader );
	}

	{
		messaging_client messaging( g );
		messaging.connect( *( server_connection_info* )NULL );
		messaging.disconnect( );
		messaging.on_message_typed( L"text", ( messaging::message_channel_enum )any_u32 );
		messaging.assign_match_channel_order( any_u32, ( game_team_id )any_u32 );
		messaging.query_for_friend_list( );
		messaging.query_for_friends_status( );
		messaging.query_for_ignore_list( );
		messaging.add_to_friend_list( any_u32 );
		messaging.remove_from_friend_list( any_u32 );
		messaging.add_to_ignore_list( any_u32 );
		messaging.remove_from_ignore_list( any_u32 );
		messaging.find_players_by_name( "name" );

		// the private packet-handling / read surface has no public caller until the
		// full login flow is wired - reference it directly (the anchor is a friend
		// of messaging_client) so /OPT:REF keeps the carcass objects
		network_core::packet_reader& reader = *( network_core::packet_reader* )NULL;
		messaging.sign_in_on_packet_received( reader );
		messaging.on_packet_received( reader );
		messaging.on_connected( );
		messaging.on_disconnected( );
		messaging.on_error( ( network_core::client_error_codes_enum )0, boost::system::error_code( ) );
		messaging.read_friend_list( reader );
		messaging.read_friend_status( reader );
		messaging.read_ignore_list( reader );
		messaging.read_found_players( reader );
		messaging.process_incoming_text_message( reader );
		messaging.accept_message_from( any_u32, ( messaging::client_type_enum )any_u32 );
		messaging.update_channel_subscriptions( );
		messaging.parse_receiver_channel( L"name", any_u32 != 0 );
	}

	{
		match_client match( world );
		match.connect( "host", ( u16 )any_u32, any_u32, any_u32, boost::function< void( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, lobby_server_message_types_enum ) >( ) );
		match.disconnect( );
		match.enqueue( NULL );
		match.send_queued_packets( any_u32 );
	}

}

} // namespace survarium
