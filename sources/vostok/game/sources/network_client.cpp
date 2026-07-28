////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_client.h"

#include <boost/bind.hpp>

// m_local_player (player_ptr) / m_net_players (boost::array<player_desc>)
// instantiate resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

#include "game.h"			// m_game.get_game_world() / m_game.lobby_menu()
#include "login_menu_status_enum.h"	// on_connected_to_login -> game::switch_to_login( status )
#include <vostok/console_command.h>	// console_commands::cc_delegate

namespace vostok {
namespace network_core {
	// network_core_entry_point.h pulls boost::asio; forward-declare the one free
	// function on_http_result_ready needs
	bool get_connection_info_from_string( pcstr buffer, char* const dest_host, u16& dest_port );
} // namespace network_core
} // namespace vostok

namespace survarium {

// TU static console command (compiler-generated atexit destructor); a matcher
// recovers its type/initializer from the init asm when this TU is enabled.
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_show_network_statistics_comand''( )
{
	// FUNCTION BODY[0x7f0290]
	// <0x7d84f0>|0x000|      :'26'	{
	// ******
}
*/

// claude@NOTE: the bind setters + member inits are structurally exact. m_is_player_ticked /
// m_is_time_synchronized_first_time are in the member-init list (the target attributes them to
// the ctor decl line, not body assignments) - that closed a 2-statement QUANTITY gap. The one
// remaining base-only `if ( m_is_spectator )` breakpoint is a /Od line-table scheduling artifact:
// the target folds the m_is_spectator store into the following guard test (the line-56 statement
// disassembles to `cmp [ebp+4184h],bl; je`), but our build - where is_spectator is a live param,
// not the anchor's const false - emits the store and the test as separate breakpoints. The
// boost::function::operator= inlining and that fold lift when the real game owner constructs the
// client (is_spectator const-propagated) and the anchor is removed (same wall as base_network_client's ctor).
 network_client::network_client( game& g, const bool is_spectator ) :
	base_network_client( g ),
	m_login_client( g.get_network_world( ) ),
	m_lobby_client( g ),
	m_match_client( g.get_network_world( ) ),
	m_messaging_client( g ),
	m_http_client( g.get_network_world( ) ),
	m_is_player_ticked( false ),
	m_is_time_synchronized_first_time( false )
{
	m_match_client.set_on_packet_received	( boost::bind( &network_client::on_match_packet_received, this, _1, _2 ) );
	m_lobby_client.set_on_packet_received	( boost::bind( &network_client::on_lobby_packet_received, this, _1 ) );
	m_lobby_client.set_on_connected			( boost::bind( &network_client::on_connected_to_lobby, this ) );
	m_lobby_client.set_on_disconnected		( boost::bind( &network_client::on_disconnected_from_lobby, this ) );
	m_http_client.set_on_error				( boost::bind( &network_client::on_http_error, this, _1 ) );

	m_is_spectator						= is_spectator;

	if ( m_is_spectator )
	{
		static console_commands::cc_delegate s_attach_to_player(
			"attach_to_player",
			boost::bind( &base_network_client::attach_to_player_cc, this, _1 ),
			true,
			console_commands::command_type_engine_internal
		);

		static console_commands::cc_delegate s_detach_to_player(
			"detach_from_player",
			boost::bind( &base_network_client::detach_from_player, this ),
			false,
			console_commands::command_type_engine_internal
		);
	}
}

// claude@NOTE: the body is the empty `{}` the target compiled - all member destruction is
// compiler-generated. The target emits 5 inlined flash_text member dtors (Scaleform::
// RefCountNTSImpl::Release on m_*_caption/value at 0x413C..0x4160); our base emits 0 because
// flash_text's dtor is a `{}` stub in this tree (the real Scaleform Release glue is still
// stubbed) - the 5 statements come back once that glue is built. Source shape is correct.
 network_client::~network_client( )
{
}

// claude@NOTE: target walks m_net_players by pointer (no index local) and reads the
// player_desc's resource_ptr directly (inlined c_ptr), not get_player(i); it records
// 0 named locals. The 0-local pointer-walk over boost::array below reproduces that
// structure (3 statements). Residual bytes are the resource_ptr/intrusive c_ptr LTCG
// inlining shared with the other player-touching handlers in this TU.
void network_client::apply_use_physics_controller_for_current( )
{
	for ( boost::array< player_desc, 20 >::const_iterator it = m_net_players.begin( ); it != m_net_players.end( ); ++it )
		if ( it->player )
			player_ptr( static_cast< player* >( it->player.c_ptr( ) ) )->set_use_physics_controller_for_current( m_use_physics_controller_for_current );
}

// claude@NOTE: the nested connection/handshaking/socket/message_type switch is structurally
// exact (28 statements, --view structure-diff is a clean shape match against the target). The
// per-statement byte residual is shared with every other LOG_ERROR site in the engine: each
// LOG_ERROR expands to the g_log_filter_tree / has_passed_filters / stored_vtable / append
// machinery, which is inlined slightly smaller here than in the target (the log-callback boost
// glue is partially stubbed). on_connected_to_login below has the same wall.
void network_client::on_connected_to_match(
	const connection_error_types_enum		connection_error,
	const handshaking_error_types_enum		handshaking_error,
	const socket_error_types_enum			socket_error,
	const lobby_server_message_types_enum	message_type
)
{
	switch ( connection_error )
	{
	case successfully_connected:
		switch ( handshaking_error )
		{
		case successfully_handshaked:
		case no_handshake:
			switch ( socket_error )
			{
			case no_socket_error:
				switch ( message_type )
				{
				case connection_successful:
					if ( !m_is_spectator )
						lobby_client( ).disconnect( );

					m_player_inputs.clear( );
					m_match_client.enqueue( m_match_client.new_packet( ( match_client_message_types_enum )0x41 ) );
					m_match_client.m_are_there_any_packets_to_send = true;
					break;

				case invalid_session_id:
					LOG_ERROR( "game: invalid session id" );
					break;

				case invalid_password:
					LOG_ERROR( "game: invalid password" );
					break;

				default:
					LOG_ERROR( "game: unexpected message type" );
					break;
				}
				break;

			case unable_to_write_to_socket:
				LOG_ERROR( "game: unable to write to socket" );
				break;

			case unable_to_read_from_socket:
				LOG_ERROR( "game: unable to read from socket" );
				break;

			default:
				LOG_ERROR( "game: unexpected socket error type" );
				break;
			}
			break;

		case cannot_handshake:
			LOG_ERROR( "game: SSL certificate verification failed" );
			break;

		default:
			LOG_ERROR( "game: unexpected SSL error" );
			break;
		}
		break;

	case cannot_connect:
		LOG_ERROR( "game: cannot connect to server" );
		break;

	default:
		LOG_ERROR( "game: unexpected socket error type" );
		break;
	}
}

void network_client::on_connected_to_login(
	const connection_error_types_enum		connection_error,
	const handshaking_error_types_enum		handshaking_error,
	const socket_error_types_enum			socket_error,
	const login_server_message_types_enum	message_type
)
{
	switch ( connection_error )
	{
	case successfully_connected:
		switch ( handshaking_error )
		{
		case successfully_handshaked:
		case no_handshake:
			switch ( socket_error )
			{
			case no_socket_error:
				{
					login_menu_status_enum status;
					switch ( message_type )
					{
					case servers_connection_info_message_type:
						LOG_INFO( "on_connected_to_login." );
						m_game.switch_to_lobby( );
						return;

					case sign_in_user_already_signed_in:
						LOG_ERROR( "sign in: user already signed in" );
						status = login_menu_status_sign_in_already_online;
						break;

					case sign_in_invalid_version:
						LOG_ERROR( "sign in: invalid version" );
						status = login_menu_status_invalid_version;
						break;

					case invalid_user_name_or_password_message_type:
						LOG_ERROR( "sign in: invalid user name or password" );
						status = login_menu_status_invalid_user_or_password;
						break;

					case user_banned_message_type:
						LOG_ERROR( "sign in: user banned" );
						status = login_menu_status_user_banned;
						break;

					case user_restricted_by_access_level_message_type:
						LOG_ERROR( "sign in: user access level restriction" );
						status = login_menu_status_access_level_restriction;
						break;

					case sign_in_attempt_interval_violated_message_type:
						LOG_ERROR( "sign in: attempt interval is violated" );
						status = login_menu_status_sign_in_attempt_interval_violated;
						break;

					default:
						LOG_ERROR( "sign in: unexpected message type" );
						status = login_menu_status_error_connection;
						break;
					}
					m_game.switch_to_login( status );
				}
				break;

			case unable_to_write_to_socket:
				LOG_ERROR( "game: unable to write to socket" );
				break;

			case unable_to_read_from_socket:
				LOG_ERROR( "game: unable to read from socket" );
				break;

			default:
				LOG_ERROR( "game: unexpected socket error type" );
				break;
			}
			break;

		case cannot_handshake:
			LOG_ERROR( "game: SSL certificate verification failed" );
			break;

		default:
			LOG_ERROR( "game: unexpected SSL error" );
			break;
		}
		break;

	case cannot_connect:
		LOG_ERROR( "game: cannot connect to login server" );
		break;

	default:
		LOG_ERROR( "game: unexpected socket error type" );
		break;
	}
}


// claude@NOTE: PARKED on a genuinely-missing cross-unit symbol. The recovered body is
//   fixed_string< 512 > client_str;
//   if ( s_net_login_client.is_set_as_string( &client_str ) ) {
//       if ( m_http_client.busy( ) )
//           return false;
//       char request_str[ 512 ];
//       sprintf_s( request_str, "%s&type=%d&local_ip=%s&login_ip=%s",
//           m_login_client.server_browser_initial_query( ), type,
//           login_client( ).local_ip_address( ), login_client( ).host_ip_address( ) );
//       m_http_client.get( m_login_client.server_browser_address( ), request_str,
//           boost::bind( &network_client::on_http_result_ready, this, _1, type ) );
//   } else {
//       if ( type == 2 ) on_http_result_ready( "188.93.23.27:25101", 2 );
//       else if ( type == 4 ) on_http_result_ready( "188.93.23.27:25102", 4 );
//   }
//   return true;
// Blocker: `s_net_login_client` is the global `vostok::command_line::key` whose canonical
// home (game.cpp) still parks it as a compiler-generated `dynamic initializer` STUB - the
// symbol is undefined, so referencing it would not link, and its ctor initializer cannot be
// recovered from the PDB to define it here (and defining it here would put the symbol in the
// wrong TU). Restore the body once game.cpp materializes s_net_login_client.
// STATE[STUB]
bool network_client::http_query_server_connection_info( u8 type )
{
	VOSTOK_UNREFERENCED_PARAMETER( type );

	// buildability return
	return false;
}

void network_client::on_http_result_ready( pcstr content, u8 type )
{
	server_connection_info connection_info;
	connection_info.session_id = login_client( ).session_id( );
	strcpy_s( connection_info.password, login_client( ).account_password( ) );
	if ( network_core::get_connection_info_from_string( content, connection_info.host, connection_info.port ) )
	{
		if ( type == 2 )
			lobby_client( ).connect( connection_info );
		else if ( type == 4 )
			messaging_client( ).connect( connection_info );
	}
	else
	{
		if ( type == 2 )
		{
			lobby_client( ).connection_info( ).need_resolve = true;
			++lobby_client( ).connection_info( ).connection_error_count;
		}
		else if ( type == 4 )
		{
			messaging_client( ).connection_info( ).need_resolve = true;
			++messaging_client( ).connection_info( ).connection_error_count;
		}
	}
}

void network_client::on_http_error( boost::system::error_code __formal )
{
	LOG_ERROR( "http client error!" );
}

// claude@NOTE: the switch dispatch is structurally exact (target compiles the same
// 0/{1,2} range-check switch + close_current_match(false/true)). The byte residual is
// set_on_disconnect: it clears a boost::function inside m_match_client.m_client
// (network::match_client, a still-stub vostok::network type), so the assign emits a
// shorter body here than the target's inlined boost::function::operator=. Lifts when
// network_core's match_client is built.
void network_client::on_match_disconnected( network_core::disconnect_event_types_enum disconnect_event_type )
{
	m_match_client.set_on_disconnect( boost::function< void( network_core::disconnect_event_types_enum ) >( ) );

	switch ( disconnect_event_type )
	{
	case network_core::disconnected_by_timeout:
		close_current_match( false );
		break;
	case network_core::disconnected_by_connection_lost:
	case network_core::disconnected_by_initiator:
		close_current_match( true );
		break;
	}
}

void network_client::connect_to_login(
	pcstr const		host,
	const u16		port,
	pcstr			account_name,
	pcstr			account_password
)
{
	if ( m_is_spectator )
	{
		m_last_tick_time_in_ms = m_game.game_time_ms( );

		LOG_INFO( "[R] connect_to_game_server: %s: %d game time is %d", host, port, m_last_tick_time_in_ms );

		m_match_client.set_on_disconnect( boost::bind( &network_client::on_match_disconnected, this, _1 ) );
		m_match_client.connect( host, port, 0, m_last_tick_time_in_ms, boost::bind( &network_client::on_connected_to_match, this, _1, _2, _3, _4 ) );

		m_game.switch_to_scene( &m_game.get_game_world( ) );
	}
	else
		m_login_client.sign_in( host, port, account_name, account_password, boost::bind( &network_client::on_connected_to_login, this, _1, _2, _3, _4 ) );
}

game_world& network_client::get_game_world( )
{
	return m_game.get_game_world( );
}

// claude@NOTE: PARKED for budget - this is a 46-statement HUD renderer, not blocked by a
// missing symbol but unusually heavy to make faithful. Recovered structure (build it later):
//   static bool first_time = true;                                   // 458
//   if ( first_time ) {
//       flash_text_manager& mgr = m_game.get_game_world( ).text_manager( );   // [m_game+0x13C]=game_world(+0x98).m_text_manager(+0xA4)  // 459
//       // 477-482: six m_sent/m_sent_low_level/m_resent/m_received/m_received_low_level/
//       // m_received_duplicated .create( mgr, caption, start_width, ... , color ) with
//       // captions "sent :"/"sent (low level) :"/"re-sent :"/"received :"/
//       // "received (low level) :"/"received (duplicates) :", start_width 200/240/280/220/
//       // 260/300, colors 0xFFFFFF00 (yellow) / 0xFF00FFFF (cyan) alternating
//       // 484-502: four flash_text setups via mgr.create_text(...) + set_color + set_position
//       //   m_max_local_sequence_difference_caption = mgr.create_text("max seq-diff :"); set_color(0xFF,0xFF,0,0xFF); set_position(150,320)
//       //   m_max_local_sequence_difference_value   = mgr.create_text("");               set_color(0xFF,0xFF,0,0xFF); set_position(325,320)
//       //   m_unacknowledged_packets_caption        = mgr.create_text("unacknowledged :"); set_color(0,0xFF,0xFF,0xFF); set_position(150,340)
//       //   m_unacknowledged_packets_value          = mgr.create_text("");               set_color(0,0xFF,0xFF,0xFF); set_position(325,340)
//       first_time = false;
//   }
//   // 505-510: six m_sent*/m_received* .set_visible( <stats_row default> )
//   // 512-515: four flash_text set_visible( s_show_network_statistics )  (the TU-static cc bool
//   //          toggled by s_show_network_statistics_comand - that console command + bool are the
//   //          other unpaired symbols in this TU; recover them together)
//   // 518-523: six .set_text( current_time_in_ms, <m_previous_stats stream>, ... )
//   // 525-526: udp_match_stats difference = m_previous_stats - get_stats; sprintf<256>(text,"%3d packets",...)
//   // 530-534: m_max_local_sequence_difference_value.set_text / m_unacknowledged_packets_value.set_text
// The create() arg marshalling (10 params, most constant-folded) and the s_show_network_statistics
// console-command static still need pinning before this is byte-faithful. Two further walls
// confirmed this pass: (1) the flash_text assignments at 484-502 route through the flash_text
// copy-assign + DrawText refcount release (`call [vtable+0x98]`), which is still-stubbed scaleform
// flash glue (flash_text holds a raw DrawText* stub) - same byte-wall as stats_row::create; and
// (2) s_show_network_statistics must be materialized as a TU-static bool here (referenced directly
// by set_visible at 512-515). Body these (flash glue + the cc static) before reattempting.
// STATE[STUB]
void network_client::draw_stats( const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETER( current_time_in_ms );
}

game_team_id network_client::get_player_team( pcstr player_profile_name )
{
	for ( u8 i = 0; i < 20; ++i )
		if ( strcmp( m_match_client.get_match_options( ).player_profiles[ i ].profile_name, player_profile_name ) == 0 )
			return m_match_client.get_match_options( ).player_profiles[ i ].team;

	return team_undefined;
}

} // namespace survarium
