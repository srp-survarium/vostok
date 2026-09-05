// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "network_client.h"

// network_client's player_ptr / player_desc members instantiate
// resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

#include "game.h"			// m_game.get_game_world( ).game_ui HUD forwarding

#include "booby_trap_set.h"			// trap-message dispatch via inventory slots
#include "victory_items_container.h"	// on_world_sync_request: clear each container's m_victory_items
#include <vostok/console_command.h>				// setup_camera_for_warmup: cc_float3 console values
#include <vostok/physics/ray_result.h>			// setup_camera_for_warmup: closest_ray_result from ray_test
#include <vostok/physics/world.h>				// setup_camera_for_warmup: physics::world::ray_test
#include "free_fly_camera.h"						// setup_camera_for_warmup: game_camera::set_position_direction
#include <vostok/network_core/packet_reader.h>
#include <vostok/scaleform/sources/flash_movie.h>	// process_victory_item_take_or_put: get_ui( )->movie->Invoke ( hide_container_icon )
#include "login_menu_status_enum.h"					// tick: m_game.switch_to_login( login_menu_status_error_connection )
#include <vostok/game_core/hit_info.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/client_player_update.h>
#include <vostok/game_core/server_player_update.h>	// process_player_action: action.deserialize / state.transform
#include <vostok/game_core/player_initial_info.h>	// query_players: info filled per player
#include <vostok/game_core/profile_slot_enum.h>
#include <vostok/game_core/affect_event_type_enum.h>
#include <vostok/game_core/hit_affects_type_enum.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/buffer_vector.h>					// query_players: ALLOCA buffer_vectors
#include <vostok/resources_queries_result.h>		// on_players_ready: data[ i ]
#include <vostok/resources_query_result.h>			// query_result_for_user::get_unmanaged_resource
#include <boost/bind.hpp>							// query_players: bind on_players_ready callback

namespace survarium {

player_ptr network_client::get_player( const u8 id ) const
{
	return player_ptr( static_cast< player* >( m_net_players[ id ].player.c_ptr( ) ) );
}

void network_client::destroy_player_impl( const u8 id )
{
	player_ptr player = get_player( id );

	m_net_players[ id ].player = NULL;

	if ( player && player->has_been_inserted( ) )
		player->remove( );

	if ( m_local_player && m_local_player->id == id )
		m_local_player = NULL;

	if ( m_current_player && m_current_player->id == id )
	{
		m_current_player = NULL;
		m_game.get_game_world( ).game_ui.show_quick_slots( false );
	}
}

void network_client::process_match_info( network_core::packet_reader& reader )
{
	match_client( ).get_match_options( ).deserialize( reader );
	match_client( ).get_match_options( ).received_players_count = 0;
}

void network_client::on_players_ready( resources::queries_result& data, const u32 players_count )
{
	// LOG_INFO embeds the target source lines 99 and 109.
#line 99
	LOG_INFO( "network_client::on_players_ready" );

	for ( u32 i = 0; i < players_count; ++i )
	{
		player_ptr player = static_cast_resource_ptr< player_ptr >( data[ i ].get_unmanaged_resource( ) );

		m_net_players[ player->id ].player = static_cast_resource_ptr< resources::unmanaged_resource_ptr >( player );

#line 109
		LOG_INFO( "network_client::on_players_ready : %d => %s", player->id, player->is_local ? "local" : "remote" );

		if ( player->is_local )
			m_local_player = player;
	}

	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x48 ) );
	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x42 ) );
	m_match_client.send_queued_packets( m_last_tick_time_in_ms );
}
#line 90
void network_client::query_players( )
{
	struct match_options& options = match_client( ).get_match_options( );


	const u32 players_count = options.players_count;

	buffer_vector< resources::request >		requests		( ALLOCA( players_count * sizeof( resources::request ) ), players_count );
	buffer_vector< variant< 32 > >			user_datas		( ALLOCA( players_count * sizeof( variant< 32 > ) ), players_count );
	buffer_vector< variant< 32 > const* >	user_data_ptrs	( ALLOCA( players_count * sizeof( variant< 32 > const* ) ), players_count );

	for ( u8 i = 0; i < players_count; ++i )
	{
		player_initial_info info;
		info.id				= i;
		info.profile		= &match_client( ).get_match_options( ).player_profiles[ i ];
		info.is_demo_player	= false;
		info.game_scene		= &m_game.get_game_world( );

		variant< 32 > ud;
		ud.set( info );
		user_datas.push_back( ud );
		user_data_ptrs.push_back( &user_datas.back( ) );
		requests.push_back( resources::create_request( "gameplay/players/default.player", resources::player_class ) );

		m_net_players[ i ].player = NULL;
		m_net_players[ i ].is_connected = false;
	}

	m_game.load(
		match_client( ).get_match_options( ).map_name,
		requests.begin( ), requests.end( ),
		user_data_ptrs.begin( ),
		boost::bind( &network_client::on_players_ready, this, _1, players_count ) );
}

// claude@NOTE: structure matches (4 statements; player_profile::deserialize now defined
// in game_net_defines.h so this links + pairs). The byte residual is codegen-only: the
// target keeps this in ebx/edi and ends with call query_players + ret 4, while the base
// allocates esi/edi and tail-jmps query_players (last-statement tail-call). The regalloc
// + tail-call choice is whole-program-driven and not source-steerable in this single TU.
void network_client::process_player_profile( network_core::packet_reader& reader )
{
	struct match_options& options = match_client( ).get_match_options( );
	options.player_profiles[ options.received_players_count++ ].deserialize( reader );

	if ( options.received_players_count == options.players_count )
		query_players( );
}

// claude@NOTE: the HUD-forwarding packet handlers below (process_team_bases /
// process_match_time / process_respawn_timer / process_match_wait_timer /
// process_base_capture_progress / process_player_kd_stats) are structurally exact
// (read u32/u8 from the packet, forward into m_game.get_game_world( ).game_ui.set_*);
// structure-diff confirms STRUCTURE MATCH. The byte residual is the packet_reader::r<T>
// inline wall (documented in packet_reader_inline.h): r<T> inlines to a direct
// *(T*)m_pointer load in the target but compiles to an out-of-line call here, so each
// read shows as a TRGT_ONLY inlined statement. process_team_bases additionally folds
// get_game_world( ).game_ui to a single [this+18h]+0x26C add in the target where the base
// computes it across the thiscall/arg threading; the forwarding call itself is emitted.
void network_client::process_team_bases( network_core::packet_reader& reader )
{
	m_game.get_game_world( ).game_ui.initialize_base_points( reader );
}

// TU-local sign-out callback; PDB: all four params __formal (genuinely
// unused) - they would collide, left unnamed. Empty body (0 statements) = matched.
void on_signed_out(
	connection_error_types_enum,
	handshaking_error_types_enum,
	socket_error_types_enum,
	login_server_message_types_enum
)
{
}

void network_client::disconnect( )
{
	if ( m_game_status )
	{
		m_game_status = game_status_inactive;
		unload( );
		m_match_client.set_on_disconnect( boost::function< void( network_core::disconnect_event_types_enum ) >( ) );
		m_match_client.disconnect( );
	}

	m_lobby_client.disconnect( );
	m_messaging_client.disconnect( );
	m_login_client.sign_out( on_signed_out );
}

void network_client::process_player_kill( network_core::packet_reader& packet )
{
	const u8 victim_id = packet.r< u8 >( );
	const u8 killer_id = packet.r< u8 >( );
	const bool is_headshot = packet.r< bool >( );
	const u32 item_dict_id = packet.r< u32 >( );

	player_ptr player = get_player( victim_id );
	m_game.get_game_world( ).game_ui.on_player_killed( victim_id, killer_id, is_headshot, item_dict_id );

	if ( player && player->has_been_inserted( ) && player->is_alive( ) )
		player->kill( m_last_tick_time_in_ms );
}

void network_client::process_player_hit( network_core::packet_reader& packet )
{
	hit_info info;
	info.deserialize( packet );
	player_ptr player = get_player( info.being_hit );
	if ( player )
		player->apply_hit_directly( info, m_last_tick_time_in_ms );
}

void network_client::process_affect_damage_model( network_core::packet_reader& packet )
{
	player_ptr player = get_player( packet.r< u8 >( ) );
	char body_part_name[ 16 ];
	packet.r_string( body_part_name );

	hit_affects_type_enum affect = packet.r< hit_affects_type_enum >( );
	affect_event_type_enum event = packet.r< affect_event_type_enum >( );
	if ( player )
		player->apply_damage_model_affect( body_part_name, affect, event );
}

void network_client::process_player_respawn( network_core::packet_reader& packet )
{
	const u8 id = packet.r< u8 >( );
	player_ptr player = get_player( id );

	player->deserialize( packet );

	if ( !is_player_connected( id ) )
		m_net_players[ id ].is_connected = true;

	if ( player->is_local && m_game_status == game_status_inprocess && m_is_time_synchronized_first_time )
		attach_to_player( player );
}

void network_client::process_initialize_victory_items( network_core::packet_reader& packet )
{
	const s8 team_1 = packet.r< s8 >( );
	const s8 team_2 = packet.r< s8 >( );

	m_game.get_game_world( ).game_ui.set_victory_points( team_1, team_2 );

	const u8 count = packet.r< u8 >( );
	for ( u8 i = 0; i < count; ++i )
	{
		const u8 slot = packet.r< u8 >( );
		const u8 item_id = packet.r< u8 >( );
		float3 position = packet.r< float3 >( );

		if ( slot == 0xFF )
		{
			float4x4 transform;
			transform = create_scale( float3( 1.f, 1.f, 1.f ) ) * create_rotation( float3( 0.f, 0.f, 0.f ) ) * create_translation( position );
			m_game.get_game_world( ).put_victory_item( item_id, transform );
		}
		else
		{
			victory_item_ptr item = m_game.get_game_world( ).get_victory_items( )[ item_id ];
			get_player( slot )->inventory( ).set_victory_item( item.c_ptr( ) );
			if ( m_current_player && m_current_player->id == slot )
				m_game.get_game_world( ).game_ui.show_item_container( slot );
		}
	}

	const u8 containers_count = packet.r< u8 >( );
	for ( u8 i = 0; i < containers_count; ++i )
	{
		const u8 container_id = packet.r< u8 >( );

		victory_items_container_core* container = m_game.get_game_world( ).get_project( )->get_items_container( container_id );

		const u8 items_count = packet.r< u8 >( );
		for ( u8 j = 0; j < items_count; ++j )
		{
			const u8 victory_item_id = packet.r< u8 >( );

			victory_item_ptr item = m_game.get_game_world( ).get_victory_items( )[ victory_item_id ];
			if ( item->is_inserted( ) )
				item->take( );

			container->put_item( item.c_ptr( ) );
		}
	}
}

void network_client::process_base_capture_progress( network_core::packet_reader& packet )
{
	const u32 progress = packet.r< u32 >( );
	const u32 point_id = packet.r< u32 >( );
	m_game.get_game_world( ).game_ui.set_base_capture_progress( progress, point_id );
}

void network_client::process_match_time( network_core::packet_reader& packet )
{
	m_game.get_game_world( ).game_ui.set_match_time( packet.r< u32 >( ) );
}

void network_client::process_respawn_timer( network_core::packet_reader& packet )
{
	m_game.get_game_world( ).game_ui.set_respawn_time( packet.r< u32 >( ) );
}

void network_client::process_match_wait_timer( network_core::packet_reader& packet )
{
	const u32 time_left = packet.r< u32 >( );
	m_game.get_game_world( ).game_ui.set_pregame(
		m_game_status == game_status_final_countdown ? "st_final_countdown" : "st_waiting_for_players", time_left );
}

// TU console values backing setup_camera_for_warmup: cc_float3 console commands
// (the warmup camera offset relative to the local player); the warmup function
// reads each via the float3 value the cc_float3 references.
static float3							s_warmup_camera_position( 1.f, 2.f, 3.f );
static float3							s_warmup_camera_target( 0.f, 1.5f, 0.f );
static console_commands::cc_float3		cc_warmup_camera_position(
											"warmup_camera_position", s_warmup_camera_position,
											float3( -1000, -1000, -1000 ), float3( 1000, 1000, 1000 ),
											true, console_commands::command_type_engine_internal );
static console_commands::cc_float3		cc_warmup_camera_target(
											"warmup_camera_target", s_warmup_camera_target,
											float3( -1000, -1000, -1000 ), float3( 1000, 1000, 1000 ),
											true, console_commands::command_type_engine_internal );

void network_client::setup_camera_for_warmup( )
{
	float3 target = m_local_player->get_current( ).transform.transform_position( s_warmup_camera_target );
	float3 position = m_local_player->get_current( ).transform.transform_position( s_warmup_camera_position );

	float3 direction = position - target;
	const float length = direction.length( );

	physics::closest_ray_result ray_result = m_game.get_game_world( ).get_physics_world( )->ray_test( position, -( direction / length ), length, 16, 8 );
	if ( ray_result.object )
		position = ray_result.hit_point_world + direction * 0.01f;

	m_game.get_game_world( ).get_free_fly_camera( )->set_position_direction( position, direction );
}

void network_client::process_game_status( network_core::packet_reader& packet )
{
	const game_status status = ( game_status )packet.r< u32 >( );

	if ( m_game_status != status )
	{
		game_world_ui& ui = m_game.get_game_world( ).game_ui;

		if ( status == game_status_inprocess )
		{
			ui.show_pregame( false );
			if ( m_local_player && m_is_time_synchronized_first_time )
				ui.show_parametrized_message( "st_start_match_welcome_message", 0, 0, 0 );
			attach_to_player( player_ptr( ) );
		}
		else
		{
			if ( !m_game_status )
			{
				ui.show_pregame( true );
				if ( m_local_player )
					setup_camera_for_warmup( );
			}
		}

		m_game_status = status;
	}
}

void network_client::process_player_kd_stats( network_core::packet_reader& packet )
{
	const u8 player_id = packet.r< u8 >( );
	const u32 kills = packet.r< u32 >( );
	const u32 deaths = packet.r< u32 >( );
	m_game.get_game_world( ).game_ui.set_player_kills_deaths( player_id, kills, deaths );
}

// claude@NOTE: STRUCTURE match (35 target / 32 base statements). The 3-statement gap and
// the byte residual are cross-TU whole-program-inline walls the single-TU base cannot
// reproduce: packet_reader::r<T> (out-of-line calls vs the target's inlined byte reads),
// simple_game_project::get_items_container (out-of-line call vs the target's inlined
// m_victory_items_containers search loop), and inventory_holder::inventory() ([ecx+8] vs
// call). team_2_points is read for the cursor advance only (recorded as a named local).
// The add_victory_points sign ( slot ? -1 : 1 ) is CSE'd once in the target but per-arg in
// the base - an LTCG scheduling artifact, not a structure divergence.
void network_client::process_victory_item_take_or_put( network_core::packet_reader& packet )
{
	const s8 team_1_points = packet.r< s8 >( );
	const s8 team_2_points = packet.r< s8 >( );

	const u8 slot = packet.r< u8 >( );
	const u8 item_id = packet.r< u8 >( );

	if ( slot == 0 && item_id == 0xFF )
		packet.r< float3 >( );

	victory_item_ptr item = m_game.get_game_world( ).get_victory_items( )[ team_1_points ];

	victory_items_container_core* current = m_game.get_game_world( ).get_project( )->get_items_container( item_id );

	if ( current )
		m_game.get_game_world( ).game_ui.add_victory_points(
			current->team( ) != team_1 ? ( slot ? -1 : 1 ) : 0,
			current->team( ) != team_2 ? ( slot ? -1 : 1 ) : 0 );

	if ( slot )
	{
		get_player( slot )->inventory( ).set_victory_item( item.c_ptr( ) );

		if ( m_current_player && m_current_player->id == slot )
			m_game.get_game_world( ).game_ui.show_item_container( slot );

		if ( !current )
		{
			item->take( );
			item->set_spotted_to_team( team_undefined );
		}
		else
			current->take_item( );
	}
	else
	{
		player_ptr current_player = get_player( slot );
		current_player->inventory( ).set_victory_item( NULL );

		if ( m_current_player && m_current_player->id == slot )
			m_game.get_game_world( ).game_ui.get_ui( )->movie->Invoke( "root.hide_container_icon", NULL, NULL, 0 );

		if ( !current )
		{
			item->set_spotted_to_team( current_player->team( ) );

			float4x4 item_transform = current_player->get_current( ).transform;
			m_game.get_game_world( ).get_victory_items( )[ team_1_points ]->put(
				m_game.get_game_world( ).get_physics_world( ), item_transform, m_game.get_game_world( ).get_game( ).scheduler( ) );
		}
		else
		{
			item->set_spotted_to_team( team_undefined );
			current->put_item( item.c_ptr( ) );
		}
	}

	m_game.get_game_world( ).game_ui.on_victory_item_put_take( item_id, slot != 0, current != NULL );
}

void network_client::send_sync_request( )
{
	m_last_sync_request_time = m_game.permanent_timer( ).get_elapsed_msec( );
	network_core::udp_match_packet* packet = m_match_client.new_packet( ( match_client_message_types_enum )0x45 );
	packet->append( m_last_sync_request_time );
	m_match_client.enqueue( packet );
}

void network_client::process_sync_response( network_core::packet_reader& packet )
{
	m_server_latency = ( m_game.permanent_timer( ).get_elapsed_msec( ) - m_last_sync_request_time ) / 2;

	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x46 ) );

	m_is_time_synchronized_first_time = true;
	if ( !m_current_player && m_game_status == game_status_inprocess && m_local_player && m_local_player->is_alive( ) )
		attach_to_player( player_ptr( ) );

	const u32 connected_mask = packet.r< u32 >( );
	for ( u8 i = 0; i < 20; ++i )
		m_net_players[ i ].is_connected = ( connected_mask & ( 1 << i ) ) != 0;
}

void network_client::send_local_player_input(
	player_input const&		input,
	const u32				time_in_ms,
	float4x4 const&			transform,
	const float				look_pitch
)
{
	if ( m_player_inputs.size( ) == m_player_inputs.max_size( ) )
		m_player_inputs.erase( m_player_inputs.begin( ) );

	m_player_inputs.push_back( client_player_update( ) );
	client_player_update& update = m_player_inputs.back( );
	update.input				= input;
	update.state.transform		= transform;
	update.state.look_pitch		= look_pitch;
	update.time_in_ms			= time_in_ms;
}

// claude@NOTE: STRUCTURE match (11/11 statements). Byte residual is the cross-TU inline
// wall: base_player::is_alive() / set_character_transform are direct out-of-line calls here
// but the target inlines them (the m_is_alive load and the transform setup) so its reads /
// member accesses fold into the surrounding statements (TRGT_ONLY rows). server_player_update
// ::deserialize is bodied now so the deserialize call is emitted. Lifts when whole-program
// inlining is reproduced.
void network_client::process_player_action( network_core::packet_reader& packet, const u32 time_in_ms )
{
	const u8 id = packet.r< u8 >( );
	player_ptr player = get_player( id );

	server_player_update action;
	action.deserialize( packet );

	if ( !player )
	{
		LOG_WARNING( "player not found %d", id );
		return;
	}

	if ( !player->is_alive( ) )
	{
		player->set_character_transform(
			action.state.transform.c.xyz( ),
			action.state.transform.get_angles( math::rotation_zxy ).y,
			action.state.look_pitch );
		return;
	}

	player->time_warp( action, time_in_ms );
}

void network_client::send_player_inputs( )
{
	for ( client_player_update* update = m_player_inputs.begin( ); update != m_player_inputs.end( ); ++update )
	{
		network_core::udp_match_packet* packet = m_match_client.new_packet( ( match_client_message_types_enum )0x43 );
		update->serialize( *packet );
		m_match_client.enqueue( packet );
	}
	m_player_inputs.clear( );
}

// PARKED: full per-frame client pump, including lobby/messaging reconnect throttles, match-client
// disconnect/connect/send/sync cadences, the per-player walk, and both camera-update paths.
// Target control flow continues after switch_to_login; there is no early return in that arm.
// The loop-local player guard uses c_ptr() because target tests only the stored pointer before
// has_been_inserted(), unlike the intrusive pointer safe-bool sequence emitted for member guards.
// The first remaining flow divergence is the loop-local resource_ptr destruction: target calls
// resource_ptr<player>::~resource_ptr out of line, while this compiler context expands its
// reference-count/destroy path in place. Both paths reconverge at the loop increment.
// Other residuals are whole-program context: login/lobby/match/messaging accessors, the game-layer
// send_queued_packets wrapper stores, player accessors, camera-update tail merging, and logging.
// Reopen only when resource_ptr destructor ownership or one of those callee contexts changes.
// The four PDB locals are current_time_in_ms, is_game_paused, id, and player.
// set_broken_connection_message's argument is VOSTOK_UNREFERENCED, so its literal is eliminated.
// Preserve this comment line count: LOG_WARNING below embeds the physical source line.
// This pass stops at that demonstrated compiler-context boundary.
void network_client::tick( const u32 current_time_in_ms, const bool is_game_paused )
{
	static u32			lobby_resolve_time		= 0;
	static u32			messaging_resolve_time	= 0;
	static const u32	min_time_delta_in_ms	= 33;

	m_last_tick_time_in_ms = current_time_in_ms;
	draw_stats( current_time_in_ms );

	if ( login_client( ).is_signed_in( ) )
	{
		if ( lobby_client( ).connection_info( ).need_resolve )
		{
			if ( lobby_client( ).connection_info( ).connection_error_count > 3 )
			{
				lobby_client( ).connection_info( ).connection_error_count = 0;
				login_client( ).sign_out( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) >( ) );
				m_game.switch_to_login( login_menu_status_error_connection );
			}
			else if ( current_time_in_ms - lobby_resolve_time > 5000 )
			{
				lobby_client( ).connection_info( ).need_resolve =
					!http_query_server_connection_info( 2 );
				lobby_resolve_time = current_time_in_ms;
				LOG_WARNING( "LOBBY: try reconnect" );
			}
		}
		else
		{
			if ( messaging_client( ).connection_info( ).need_resolve )
				if ( current_time_in_ms - messaging_resolve_time > 3000 )
				{
					messaging_client( ).connection_info( ).need_resolve =
						!http_query_server_connection_info( 4 );
					messaging_resolve_time = current_time_in_ms;
				}
		}
	}

	if ( match_client( ).is_disconnected( ) )
	{
		m_player_inputs.clear( );
		return;
	}

	if ( !match_client( ).is_connected( ) )
	{
		m_player_inputs.clear( );

		if ( current_time_in_ms - m_last_send_queued_packets_time_in_ms >= min_time_delta_in_ms )
		{
			m_last_send_queued_packets_time_in_ms = current_time_in_ms;
			match_client( ).send_queued_packets( current_time_in_ms );
		}
		return;
	}

	if ( match_client( ).last_receive_time_in_ms( ) + 3000 <= current_time_in_ms )
	{
		m_player_inputs.clear( );
		m_game.get_game_world( ).game_ui.set_broken_connection_message( "match server connection lost" );
	}

	if ( m_last_player_input_send_time_in_ms + min_time_delta_in_ms <= current_time_in_ms )
	{
		m_last_player_input_send_time_in_ms = current_time_in_ms / min_time_delta_in_ms * min_time_delta_in_ms;
		send_player_inputs( );
	}

	if ( m_is_time_synchronized_first_time && current_time_in_ms - m_last_sync_request_time > 4000 )
		send_sync_request( );

	if ( match_client( ).are_there_any_packets_to_send( ) ||
		match_client( ).last_send_queed_packets_time_in_ms( ) + min_time_delta_in_ms <= current_time_in_ms )
		match_client( ).send_queued_packets( current_time_in_ms );

	if ( is_game_paused && m_current_player && m_current_player->has_been_inserted( ) )
	{
		m_current_player->update_camera( );
		return;
	}

	for ( u8 id = 0; id < 20; ++id )
	{
		player_ptr player = get_player( id );
		if ( player.c_ptr( ) && player->has_been_inserted( ) )
		{
			if ( !m_is_player_ticked && player->is_local )
			{
				m_is_player_ticked = true;
				send_sync_request( );
			}

			if ( m_is_time_synchronized_first_time || !player->is_local )
				player->tick( current_time_in_ms );
		}
	}

	if ( m_current_player && m_current_player->has_been_inserted( ) )
		m_current_player->update_camera( );
}

void network_client::initiate_kill_current_player( )
{
	if ( m_local_player.c_ptr( ) && m_current_player.c_ptr( ) )
		m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x44 ) );
}

void network_client::unload( )
{
	for ( u8 id = 0; id < 20; ++id )
		if ( get_player( id ) )
			destroy_player_impl( id );
}

bool network_client::is_player_local( const u8 player_id ) const
{
	return m_local_player && m_local_player->id == player_id;
}

// claude@NOTE: STRUCTURE match. The TRGT_ONLY rows are resource_ptr<player> destructor
// cleanup blocks (lock xadd refcount + unmanaged_intrusive_base::destroy) the target whole-
// program-inlines at each early-return tail, plus the packet_reader::r<T> inline split; the
// base emits a single out-of-line ~resource_ptr / r<T> call. Lifts with networking inlining.
void network_client::player_visibility_change( network_core::packet_reader& packet )
{
	const u8 id = packet.r< u8 >( );
	const bool is_visible = packet.r< bool >( );
	player_ptr player = get_player( id );
	if ( m_local_player && player )
		if ( player->has_been_inserted( ) && is_visible != player->is_visible( ) )
			is_visible ? player->show( ) : player->hide( );
}

// claude@NOTE: the four on_trap_* handlers read three packet bytes (player id, slot
// index, trap index) then dispatch via the player's inventory slot. The byte residual
// vs target is the packet_reader::r<T> / inventory()/item_in_slot() whole-program-inline
// wall (target inlines to direct member loads; this single-TU base emits the out-of-line
// reads) - documented in packet_reader_inline.h; lifts when networking inlining is whole.
void network_client::on_trap_placed( network_core::packet_reader& packet )
{
	const u8 player_id = packet.r< u8 >( );
	const u8 slot = packet.r< u8 >( );
	const u8 index = packet.r< u8 >( );
	float3 position = packet.r< float3 >( );
	float3 angles = packet.r< float3 >( );
	player_ptr player = get_player( player_id );
	static_cast< booby_trap_set* >( player->inventory( ).item_in_slot( ( profile_slot_enum )slot ).c_ptr( ) )
		->on_trap_placed_message( index, position, angles );
}

void network_client::on_trap_removed( network_core::packet_reader& packet )
{
	const u8 player_id = packet.r< u8 >( );
	const u8 slot = packet.r< u8 >( );
	const u8 index = packet.r< u8 >( );
	player_ptr player = get_player( player_id );
	static_cast< booby_trap_set* >( player->inventory( ).item_in_slot( ( profile_slot_enum )slot ).c_ptr( ) )
		->on_trap_removed_message( index );
}

void network_client::on_trap_fired( network_core::packet_reader& packet )
{
	const u8 player_id = packet.r< u8 >( );
	const u8 slot = packet.r< u8 >( );
	const u8 index = packet.r< u8 >( );
	player_ptr player = get_player( player_id );
	static_cast< booby_trap_set* >( player->inventory( ).item_in_slot( ( profile_slot_enum )slot ).c_ptr( ) )
		->on_trap_fired_message( index );
}

void network_client::on_trap_disarmed( network_core::packet_reader& packet )
{
	const u8 player_id = packet.r< u8 >( );
	const u8 slot = packet.r< u8 >( );
	const u8 index = packet.r< u8 >( );
	player_ptr player = get_player( player_id );
	static_cast< booby_trap_set* >( player->inventory( ).item_in_slot( ( profile_slot_enum )slot ).c_ptr( ) )
		->on_trap_disarmed_message( index );
}

void network_client::game_world_object_state_arrived( network_core::packet_reader& reader )
{
	player_ptr player = get_player( reader.r< u8 >( ) );
	player->deserialize_game_world_object( reader );
}

void network_client::on_world_sync_request( )
{
	m_player_inputs.clear( );

	for ( u8 id = 0; id < 20; ++id )
	{
		player_ptr player = get_player( id );
		if ( !player || !player->has_been_inserted( ) )
			continue;

		player->remove( );
		if ( player->inventory( ).get_victory_item( ) )
			player->inventory( ).set_victory_item( NULL );
	}

	for ( vector< victory_items_container* >::iterator i = m_game.get_game_world( ).get_project( )->m_victory_items_containers.begin( );
		i != m_game.get_game_world( ).get_project( )->m_victory_items_containers.end( ); ++i )
		( *i )->m_victory_items.clear( );

	for ( vectora< victory_item_ptr >::iterator it = m_game.get_game_world( ).get_victory_items( ).begin( );
		it != m_game.get_game_world( ).get_victory_items( ).end( ); ++it )
		if ( ( *it )->is_inserted( ) )
			( *it )->take( );

	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x4A ) );
}

void network_client::damage_model_state_arrived( network_core::packet_reader& packet )
{
	player_ptr player = get_player( packet.r< u8 >( ) );
	player->damage_model( )->deserialize( packet );
}

} // namespace survarium
