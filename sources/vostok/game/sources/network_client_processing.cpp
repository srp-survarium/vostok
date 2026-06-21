////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

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

// claude@NOTE: on_players_ready / query_players are STRUCTURE matches; the byte residual
// is the resources inlining wall this single-TU base cannot reproduce - the variant<32>
// machinery, buffer_vector::push_back, the resource_ptr refcount blocks and boost::bind
// are out-of-line here but whole-program-inlined in the target. query_players' tail also
// emits one extra target instruction (m_game.m_lpv_geometry_builded = false, a private
// game member that needs friendship not yet in game.h) that is not reproduced. The two
// on_players_ready LOG_INFO format strings are guessed (the exact text lives only in the
// shipped rdata; it does not change the instruction bytes).
void network_client::on_players_ready( resources::queries_result& data, const u32 players_count )
{
	LOG_INFO( "network_client::on_players_ready" );

	for ( u32 i = 0; i < players_count; ++i )
	{
		player_ptr player = static_cast_resource_ptr< player_ptr >( data[ i ].get_unmanaged_resource( ) );

		m_net_players[ player->id ].player = static_cast_resource_ptr< resources::unmanaged_resource_ptr >( player );

		LOG_INFO( "network_client::on_players_ready %s player created %d", player->is_local ? "local" : "remote", player->id );

		if ( player->is_local )
			m_local_player = player;
	}

	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x48 ) );
	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x42 ) );
	m_match_client.send_queued_packets( m_last_tick_time_in_ms );
}

void network_client::query_players( )
{
	const u32 players_count = match_client( ).get_match_options( ).players_count;

	buffer_vector< resources::request >		requests		( ALLOCA( players_count * sizeof( resources::request ) ), players_count );
	buffer_vector< variant< 32 > >			user_datas		( ALLOCA( players_count * sizeof( variant< 32 > ) ), players_count );
	buffer_vector< variant< 32 > const* >	user_data_ptrs	( ALLOCA( players_count * sizeof( variant< 32 > const* ) ), players_count );

	for ( u8 i = 0; i < players_count; ++i )
	{
		player_initial_info info;
		info.profile		= &match_client( ).get_match_options( ).player_profiles[ i ];
		info.id				= i;
		info.game_scene		= &m_game.get_game_world( );
		info.is_demo_player	= false;

		user_datas.push_back( variant< 32 >( ) );
		user_datas.back( ).set( info );
		user_data_ptrs.push_back( &user_datas.back( ) );
		requests.push_back( resources::create_request( "gameplay/players/default.player", resources::player_class ) );

		m_net_players[ i ].player = NULL;
		m_net_players[ i ].is_connected = false;
	}

	m_game.get_game_world( ).load(
		m_game.project_resource_name( ),
		requests.begin( ), requests.end( ),
		user_data_ptrs.begin( ),
		boost::bind( &network_client::on_players_ready, this, _1, players_count ) );
}

// claude@NOTE: parked at link, not structure. Body recovered:
//   match_options& options = match_client().get_match_options();
//   options.player_profiles[options.received_players_count++].deserialize(reader);
//   if (options.received_players_count == options.players_count) query_players();
// but player_profile::deserialize has no definition (its body is commented out in
// game_net_defines.h as a sushi@TODO - "move to player_profile's own TU"), so the call
// is an unresolved external (LNK2001) that fails the EXE link. Unblocks once
// player_profile::deserialize lands in its TU.
// STATE[STUB]
void network_client::process_player_profile( network_core::packet_reader& reader )
{
	VOSTOK_UNREFERENCED_PARAMETER( reader );
}

// claude@NOTE: the HUD-forwarding packet handlers below (process_team_bases /
// process_match_time / process_respawn_timer / process_match_wait_timer /
// process_base_capture_progress / process_player_kd_stats) are structurally exact
// (read u32/u8 from the packet, forward into m_game.get_game_world( ).game_ui.set_*);
// structure-diff confirms STRUCTURE MATCH. The byte residual is two environmental
// LTCG walls this single-TU base cannot reproduce: (1) packet_reader::r<T> inlines to
// a direct *(T*)m_pointer load in the target but compiles to an out-of-line call here
// (documented in packet_reader_inline.h), and (2) game_world_ui::initialize_base_points
// and set_base_capture_progress are still STUBs in game_world_ui.cpp, so their empty
// bodies inline away the forwarding call entirely in the base. Both lift when whole-
// program inlining is reproduced / those game_world_ui bodies are matched.
void network_client::process_team_bases( network_core::packet_reader& reader )
{
	m_game.get_game_world( ).game_ui.initialize_base_points( reader );
}

// STATE[STUB]
// TU-local sign-out callback; PDB: all four params __formal (genuinely
// unused) - they would collide, left unnamed
void on_signed_out(
	connection_error_types_enum,
	handshaking_error_types_enum,
	socket_error_types_enum,
	login_server_message_types_enum
)
{
}

// claude@NOTE: lines 200 (a network_client virtual via vtable slot 8 + m_game_status=0)
// and 202 (resetting m_match_client's boost::function<void(pcstr)> callback at +0xC8 via
// operator=) are not pinned to named members yet - the body below stands in structurally
// (if-guard + match_client.disconnect under it, then lobby/messaging/login). The byte
// residual is dominated by the boost::function operator= / sign_out callback-vtable
// machinery this single-TU base cannot reproduce verbatim.
void network_client::disconnect( )
{
	if ( m_game_status )
	{
		apply_use_physics_controller_for_current( );
		m_game_status = game_status_inactive;
		m_match_client.set_on_packet_received( boost::function< void( u8, network_core::packet_reader& ) >( ) );
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

// claude@NOTE: line 257 is an out-of-line player virtual at vtable slot 25 ([player]+0x64)
// that takes the network_client (push this), not the reader - no player override taking a
// base_network_client is identifiable yet, so deserialize(packet) stands in as the
// structurally-correct single-statement placeholder (right count/position, links). Replace
// once the slot-25 override is named. Byte residual is the shared packet_reader::r<T> wall.
void network_client::process_player_respawn( network_core::packet_reader& packet )
{
	const u8 id = packet.r< u8 >( );
	player_ptr player = get_player( id );

	player->deserialize( packet );

	if ( !m_net_players[ id ].is_connected )
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
			float4x4 transform = create_scale( float3( 1.f, 1.f, 1.f ) ) * create_rotation( float3( 0.f, 0.f, 0.f ) ) * create_translation( position );
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
				item->unload( );

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
static float3							s_warmup_camera_position( 0.f, 0.f, 0.f );
static float3							s_warmup_camera_target( 0.f, 0.f, 0.f );
static console_commands::cc_float3		cc_warmup_camera_position(
											"warmup_camera_position", s_warmup_camera_position,
											float3( -1000, -1000, -1000 ), float3( 1000, 1000, 1000 ),
											false, console_commands::command_type_user_specific );
static console_commands::cc_float3		cc_warmup_camera_target(
											"warmup_camera_target", s_warmup_camera_target,
											float3( -1000, -1000, -1000 ), float3( 1000, 1000, 1000 ),
											false, console_commands::command_type_user_specific );

void network_client::setup_camera_for_warmup( )
{
	float3 target = m_local_player->get_current( ).transform.transform_position( s_warmup_camera_target );
	float3 position = m_local_player->get_current( ).transform.transform_position( s_warmup_camera_position );

	float3 direction = position - target;
	const float length = direction.length( );

	physics::closest_ray_result ray_result = m_game.get_game_world( ).get_physics_world( )->ray_test( position, -direction / length, length, 16, 8 );
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

// claude@NOTE: PARKED - 35 statements, deeply nested take-vs-put branches + a
// Scaleform::GFx::Movie::Invoke ("root.hide_container_icon") + multiple unresolved
// victory_item/victory_items_container vtable slots (high mis-structure risk). Shape
// (lines 413-470), reusing idioms now PROVEN in process_initialize_victory_items above:
//   413 const s8 team_1_points = packet.r<s8>();  414 const s8 team_2_points = packet.r<s8>();
//   415 const u8 slot = packet.r<u8>();            416 const u8 item_id = packet.r<u8>();
//   419 if ( slot == 0 && item_id == 0xFF )  420 packet.r<float3>();  // read+discard a float3
//   422 victory_item_ptr item = m_game.get_game_world().get_victory_items()[ team_1_points ];
//        // NB: indexed by team_1_points (dl), NOT item_id - matches the binary, do not "fix"
//   424 victory_items_container_core* current =
//        m_game.get_game_world().get_project()->get_items_container( item_id );  // inlined search
//   426-430 if ( current ) game_ui.add_victory_points( ... );  // two current-> virtuals: team()
//        at vtable+0x28 (game_team_id) and one at +0x28 again, results -> add_victory_points
//   433 if ( slot ) {  // PUT path
//     434 get_player(slot)->inventory().set_victory_item( item.c_ptr() );  436-437 player dtor +
//          if ( m_current_player && m_current_player->id == slot ) game_ui.show_item_container(slot);
//     439 if ( !current ) { 441 item->take()/vtbl+0x24(); 444 item[+0x170] = 3; }
//          446 else current->take_item()/vtbl+0x24();
//   } else {  // TAKE path (slot == 0)
//     449 get_player(slot)->inventory().set_victory_item( NULL );
//     451-452 if ( m_current_player && m_current_player->id == slot )
//          [game_ui+0x270]->[+0x108]->[+4]->Scaleform::GFx::Movie::Invoke("root.hide_container_icon",0,0,0);
//     454 if ( !current ) { 455 item[+0x170] = item->vtbl+0x48()/put-return;
//          461 float4x4 item_transform = get_player(slot)->get_current().transform; (player+0x8770)
//               put it back: get_victory_items()[..]->put( physics_world, item_transform, scheduler );
//          } else { 465 item[+0x170] = 3; 466 current->put_item( item.c_ptr() ); }
//   }
//   470 game_ui.on_victory_item_put_take( item_id, slot != 0, current != NULL );
// NEXT: name the victory_item vtable slots (+0x24 take, +0x28 a game_team_id getter, +0x48 a
// put-return), the container's [+0x34]==item_id key (already get_items_container), and the GFx
// movie accessor chain [game_ui+0x270]->[+0x108]->[+4]; then write it like the sibling above.
// STATE[STUB]
void network_client::process_victory_item_take_or_put( network_core::packet_reader& packet )
{
	// LOCALS
	// s8 								team_1_points
	// s8 								team_2_points
	// player_ptr 						current_player
	// float4x4 						item_transform
	// victory_item_ptr 				item
	// ******

	// CALL SITE INFO
	// <0x5c54bc> -> game_team_id < unknown >()
	// <0x5c54d1> -> game_team_id < unknown >()
	// <0x5c5510> -> player_ptr < unknown >( const u8 ) const
	// <0x5c555c> -> void < unknown >()
	// <0x5c5574> -> victory_item_core* < unknown >()
	// <0x5c558d> -> player_ptr < unknown >( const u8 ) const
	// <0x5c55e5> -> game_team_id < unknown >() const
	// <0x5c5657> -> void < unknown >( victory_item_core* )
	// ******

	// FUNCTION BODY[0x5c53d0]: 58
	// <0x5c53d3>|0x003|+0x00a:'413'
	// <0x5c53dd>|0x00d|+0x00c:'414'
	// <0x5c53e9>|0x019|+0x010:'415'
	// <0x5c53f9>|0x029|+0x004:'416'
	// <0>
	// <1>
	// <0x5c53fd>|0x02d|+0x015:'419'
	// <0x5c5412>|0x042|+0x006:'420'
	// <0>
	// <0x5c5418>|0x048|+0x027:'422'
	// <0>
	// <0x5c543f>|0x06f|+0x066:'424'
	// <0>
	// <0x5c54a5>|0x0d5|+0x004:'426'
	// <0x5c54a9>|0x0d9|+0x005:'427'
	// <0x5c54ae>|0x0de|+0x010:'428'
	// <0x5c54be>|0x0ee|+0x015:'429'
	// <0x5c54d3>|0x103|+0x024:'430'
	// <0>
	// <1>
	// <0x5c54f7>|0x127|+0x007:'433'
	// <0x5c54fe>|0x12e|+0x02c:'434'
	// <0>
	// <0x5c552a>|0x15a|+0x019:'436'
	// <0x5c5543>|0x173|+0x00e:'437'
	// <0>
	// <0x5c5551>|0x181|+0x004:'439'
	// <0>
	// <0x5c5555>|0x185|+0x009:'441'
	// <0x5c555e>|0x18e|+0x00a:'442'
	// <0>
	// <0x5c5568>|0x198|+0x005:'444'
	// <0>
	// <0x5c556d>|0x19d|+0x009:'446'
	// <0>
	// <1>
	// <0x5c5576>|0x1a6|+0x005:'449'
	// <0>
	// <0x5c557b>|0x1ab|+0x014:'451'
	// <0x5c558f>|0x1bf|+0x00e:'452'
	// <0>
	// <0x5c559d>|0x1cd|+0x019:'454'
	// <0x5c55b6>|0x1e6|+0x022:'455'
	// <0>
	// <0x5c55d8>|0x208|+0x004:'457'
	// <0x5c55dc>|0x20c|+0x00f:'458'
	// <0>
	// <1>
	// <0x5c55eb>|0x21b|+0x050:'461'
	// <0>
	// <0x5c563b>|0x26b|+0x006:'463'
	// <0>
	// <0x5c5641>|0x271|+0x00e:'465'
	// <0x5c564f>|0x27f|+0x00a:'466'
	// <0>
	// <0x5c5659>|0x289|+0x009:'468'
	// <0>
	// <0x5c5662>|0x292|+0x022:'470'
	// ******
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

	client_player_update update;
	update.input				= input;
	update.state.transform		= transform;
	update.state.look_pitch		= look_pitch;
	update.time_in_ms			= time_in_ms;
	m_player_inputs.push_back( update );
}

// claude@NOTE: STRUCTURE match (11/11 statements). Byte residual is two cross-TU inline
// walls: player::set_character_transform is still an empty STUB in player.cpp, so its body
// inlines to nothing here and only the get_angles arg-setup survives (the call vanishes);
// and base_player::is_alive() is a direct out-of-line call here but the target inlines the
// m_is_alive load. Both lift once those callees are matched / whole-program inlining is
// reproduced. The set_character_transform position arg (transform.c.xyz()) is the best
// guess - the stubbed callee makes it unverifiable for now.
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

// claude@NOTE: PARKED - the 122-statement dispatch tick (largest in the TU). Carcass below
// retains the per-statement listing; the shape is the per-frame client pump: resolve the
// lobby/messaging clients on a throttle (the two static u32 *_resolve_time + min_time_delta
// statics), m_last_tick_time_in_ms = current_time_in_ms, then login/lobby/match/messaging
// client tick + send_queued_packets, the player-input throttle and a get_player(id) walk.
// Deferred for budget; needs the static-throttle timers, the client tick ordering and the
// send-input cadence reconstructed against the rich asm before writing. NEXT: decode the
// throttle block (lines 602-657) then the match/messaging tick + input-send cadence.
// STATE[STUB]
void network_client::tick( const u32 current_time_in_ms, const bool is_game_paused )
{
	// LOCALS
	// u8 								id
	// player_ptr 						player
	// ******

	// STATICS
	// static u32 						lobby_resolve_time = <0x4c26630>;
	// static u32 						messaging_resolve_time = <0x4c26634>;
	// static const u32 				min_time_delta_in_ms = <0x10000>;
	// ******

	// CALL SITE INFO
	// <0x5c620b> -> network::login_client& < unknown >()
	// <0x5c6221> -> lobby_client& < unknown >()
	// <0x5c6237> -> lobby_client& < unknown >()
	// <0x5c6249> -> lobby_client& < unknown >()
	// <0x5c6261> -> network::login_client& < unknown >()
	// <0x5c629d> -> lobby_client& < unknown >()
	// <0x5c6370> -> match_client& < unknown >()
	// <0x5c6395> -> messaging_client& < unknown >()
	// <0x5c63b6> -> messaging_client& < unknown >()
	// <0x5c63e3> -> match_client& < unknown >()
	// <0x5c641a> -> match_client& < unknown >()
	// <0x5c6441> -> match_client& < unknown >()
	// <0x5c64d5> -> match_client& < unknown >()
	// <0x5c6531> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c61e0]: 122
	// <0x5c61e0>|0x000|+0x00a:'594'	{
	// <0x5c61ea>|0x00a|+0x007:'595'
	// <0>
	// <0x5c61f1>|0x011|+0x013:'597'
	// <0>
	// <0x5c6204>|0x024|+0x016:'599'
	// <0>
	// <1>
	// <0x5c621a>|0x03a|+0x010:'602'
	// <0>
	// <0x5c622a>|0x04a|+0x018:'604'
	// <0>
	// <0x5c6242>|0x062|+0x00f:'606'
	// <0x5c6251>|0x071|+0x022:'607'
	// <0x5c6273>|0x093|+0x00a:'608'
	// <0x5c627d>|0x09d|+0x005:'609'
	// <0>
	// <1>
	// <0x5c6282>|0x0a2|+0x014:'612'
	// <0>
	// <0x5c6296>|0x0b6|+0x014:'614'
	// <0x5c62aa>|0x0ca|+0x00e:'615'
	// <0x5c62b8>|0x0d8|+0x0ae:'616'
	// <0>
	// <1>
	// <0x5c6366>|0x186|+0x02c:'619'
	// <0x5c6392>|0x1b2|+0x00e:'620'
	// <0>
	// <1>
	// <0x5c63a0>|0x1c0|+0x00f:'623'
	// <0>
	// <0x5c63af>|0x1cf|+0x025:'625'
	// <0x5c63d4>|0x1f4|-0x06b:'626'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c6369>|0x189|+0x014:'631'
	// <0x5c637d>|0x19d|+0x05f:'632'
	// <0>
	// <1>
	// <2>
	// <0x5c63dc>|0x1fc|+0x014:'636'
	// <0x5c63f0>|0x210|+0x00c:'637'
	// <0>
	// <0x5c63fc>|0x21c|+0x011:'639'
	// <0>
	// <0x5c640d>|0x22d|+0x02d:'641'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c643a>|0x25a|+0x019:'646'
	// <0x5c6453>|0x273|+0x00c:'647'
	// <0x5c645f>|0x27f|+0x00f:'648'
	// <0>
	// <1>
	// <2>
	// <0x5c646e>|0x28e|+0x00d:'652'
	// <0x5c647b>|0x29b|+0x011:'653'
	// <0x5c648c>|0x2ac|+0x00c:'654'
	// <0>
	// <1>
	// <0x5c6498>|0x2b8|+0x019:'657'
	// <0x5c64b1>|0x2d1|+0x007:'658'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <0x5c64b8>|0x2d8|+0x016:'686'
	// <0x5c64ce>|0x2ee|+0x01e:'687'
	// <0>
	// <0x5c64ec>|0x30c|+0x01f:'689'
	// <0>
	// <0x5c650b>|0x32b|+0x00f:'691'
	// <0>
	// <1>
	// <2>
	// <0x5c651a>|0x33a|+0x006:'695'
	// <0>
	// <0x5c6520>|0x340|+0x013:'697'
	// <0x5c6533>|0x353|+0x053:'698'
	// <0x5c6586>|0x3a6|-0x042:'699'
	// <0>
	// <1>
	// <0x5c6544>|0x364|+0x00f:'702'
	// <0>
	// <0x5c6553>|0x373|+0x012:'704'
	// <0>
	// <1>
	// <0x5c6565>|0x385|+0x00f:'707'
	// <0x5c6574>|0x394|+0x007:'708'
	// <0x5c657b>|0x39b|+0x049:'709'
	// <0>
	// <0x5c65c4>|0x3e4|-0x0b8:'711'
	// <0x5c650c>|0x32c|+0x0d1:'712'
	// <0x5c65dd>|0x3fd|-0x254:'712'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c6389>|0x1a9|+0x0a8:'717'
	// <0x5c6431>|0x251|+0x0e0:'717'
	// <0x5c6511>|0x331|+0x0d2:'717'
	// <0x5c65e3>|0x403|      :'717'	}
	// ******
}

void network_client::initiate_kill_current_player( )
{
	if ( m_local_player && get_current_player( ) )
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
	if ( m_local_player && m_local_player->id == player_id )
		return true;
	return false;
}

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
		if ( player && player->has_been_inserted( ) )
		{
			player->remove( );
			if ( player->inventory( ).get_victory_item( ) )
				player->inventory( ).set_victory_item( NULL );
		}
	}

	for ( vector< victory_items_container* >::iterator i = m_game.get_game_world( ).get_project( )->m_victory_items_containers.begin( );
		i != m_game.get_game_world( ).get_project( )->m_victory_items_containers.end( ); ++i )
		( *i )->m_victory_items.clear( );

	for ( vectora< victory_item_ptr >::iterator it = m_game.get_game_world( ).get_victory_items( ).begin( );
		it != m_game.get_game_world( ).get_victory_items( ).end( ); ++it )
		if ( ( *it )->is_inserted( ) )
			( *it )->unload( );

	m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x4A ) );
}

void network_client::damage_model_state_arrived( network_core::packet_reader& packet )
{
	player_ptr player = get_player( packet.r< u8 >( ) );
	player->damage_model( )->deserialize( packet );
}

} // namespace survarium
