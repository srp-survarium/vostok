// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "network_client.h"

#include <boost/bind.hpp>

// m_local_player (player_ptr) / m_net_players (boost::array<player_desc>)
// instantiate resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

#include "game.h"			// m_game.get_game_world() / m_game.lobby_menu()
#include "flash_text_manager.h"
#include "login_menu_status_enum.h"	// on_connected_to_login -> game::switch_to_login( status )
#include <vostok/console_command.h>	// console_commands::cc_delegate
#include <vostok/command_line_extensions.h>

extern vostok::command_line::key s_net_login_client;

namespace vostok {
namespace network_core {
	// network_core_entry_point.h pulls boost::asio; forward-declare the one free
	// function on_http_result_ready needs
	bool get_connection_info_from_string( pcstr buffer, char* const dest_host, u16& dest_port );
} // namespace network_core
} // namespace vostok

namespace survarium {

static bool s_show_network_statistics = false;
static vostok::console_commands::cc_bool s_show_network_statistics_comand(
	"net_stats",
	s_show_network_statistics,
	true,
	vostok::console_commands::command_type_user_specific
);

 network_client::network_client( game& g, const bool is_spectator ) :
	base_network_client( g ),
	m_login_client( g.get_network_world( ) ),
	m_lobby_client( g ),
	m_match_client( g.get_network_world( ) ),
	m_messaging_client( g ),
	m_http_client( g.get_network_world( ) ),
	m_is_spectator( is_spectator ),
	m_is_player_ticked( false ),
	m_is_time_synchronized_first_time( false )
{
	m_match_client.set_on_packet_received	( boost::bind( &network_client::on_match_packet_received, this, _1, _2 ) );
	m_lobby_client.set_on_packet_received	( boost::bind( &network_client::on_lobby_packet_received, this, _1 ) );
	m_lobby_client.set_on_connected			( boost::bind( &network_client::on_connected_to_lobby, this ) );
	m_lobby_client.set_on_disconnected		( boost::bind( &network_client::on_disconnected_from_lobby, this ) );
	m_http_client.set_on_error				( boost::bind( &network_client::on_http_error, this, _1 ) );
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

network_client::~network_client( )
{
	if ( flash_text_manager* const text_manager = m_game.get_game_world( ).get_text_manager( ) )
	{
		text_manager->destroy_text( m_unacknowledged_packets_value );
		text_manager->destroy_text( m_unacknowledged_packets_caption );
		text_manager->destroy_text( m_max_local_sequence_difference_value );
		text_manager->destroy_text( m_max_local_sequence_difference_caption );
	}
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


bool network_client::http_query_server_connection_info( u8 type )
{
	fixed_string< 512 > client_str;
	if ( s_net_login_client.is_set_as_string( &client_str ) )
	{
		if ( m_http_client.busy( ) )
			return false;

		pcstr const server_browser_address = m_login_client.server_browser_address( );
		pcstr const server_browser_initial_query = m_login_client.server_browser_initial_query( );
		pcstr const local_ip_address = login_client( ).local_ip_address( );
		pcstr const host_ip_address = login_client( ).host_ip_address( );

		char request_str[ 512 ];
		::sprintf_s(
			request_str,
			"%s&type=%d&local_ip=%s&login_ip=%s",
			server_browser_initial_query,
			type,
			local_ip_address,
			host_ip_address
		);
		m_http_client.get(
			server_browser_address,
			request_str,
			boost::bind( &network_client::on_http_result_ready, this, _1, type )
		);
	}
	else
	{
		if ( type == 2 )
			on_http_result_ready( "188.93.23.27:25101", 2 );
		else if ( type == 4 )
			on_http_result_ready( "188.93.23.27:25102", 4 );
	}

	return true;
}

void network_client::on_http_result_ready( pcstr content, u8 type )
{
	server_connection_info connection_info;
	connection_info.session_id = login_client( ).session_id( );
	connection_info.need_resolve = false;
	strcpy_s( connection_info.password, login_client( ).account_password( ) );
	bool const is_connection_info_valid = network_core::get_connection_info_from_string( content, connection_info.host, connection_info.port );

	if ( is_connection_info_valid )
	{
		if ( type == 2 )
			lobby_client( ).connect( connection_info );
		else
		if ( type == 4 )
			messaging_client( ).connect( connection_info );
	}
	else {
		if ( type == 2 )
		{
			lobby_client( ).connection_info( ).need_resolve = true;
			++lobby_client( ).connection_info( ).connection_error_count;
		}
		else
		if ( type == 4 )
		{
			messaging_client( ).connection_info( ).need_resolve = true;
			++messaging_client( ).connection_info( ).connection_error_count;
		}
	}

}
// LOG_ERROR bakes target source line 402 into this function.
#line 400
void network_client::on_http_error( boost::system::error_code __formal )
{
	LOG_ERROR( "http client error!" );
}
#line 405
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

void network_client::draw_stats( const u32 current_time_in_ms )
{
	static bool first_time = true;

	if ( first_time )
	{
		flash_text_manager* text_manager;
		if ( !( text_manager = m_game.get_game_world( ).get_text_manager( ) ) )
			return;

		first_time = false;

		m_sent.create(
			*text_manager, "sent :", 150.f, 200.f, 175.f,
			100.f, 100.f, 100.f, 40.f, math::color( 0xFFFFFF00 )
		);
		m_sent_low_level.create(
			*text_manager, "sent (low level) :", 150.f, 240.f, 175.f,
			100.f, 100.f, 100.f, 40.f, math::color( 0xFF00FFFF )
		);
		m_resent.create(
			*text_manager, "re-sent :", 150.f, 280.f, 175.f,
			100.f, 100.f, 100.f, 40.f, math::color( 0xFFFFFF00 )
		);
		m_received.create(
			*text_manager, "received :", 150.f, 220.f, 175.f,
			100.f, 100.f, 100.f, 40.f, math::color( 0xFF00FFFF )
		);
		m_received_low_level.create(
			*text_manager, "received (low level) :", 150.f, 260.f, 175.f,
			100.f, 100.f, 100.f, 40.f, math::color( 0xFFFFFF00 )
		);
		m_received_duplicated.create(
			*text_manager, "received (duplicates) :", 150.f, 300.f, 175.f,
			100.f, 100.f, 100.f, 40.f, math::color( 0xFF00FFFF )
		);

		m_max_local_sequence_difference_caption = text_manager->create_text( "max seq-diff :" );
		m_max_local_sequence_difference_caption.set_visible( true );
		m_max_local_sequence_difference_caption.set_color( 0, 0xFF, 0xFF, 0xFF );
		m_max_local_sequence_difference_caption.set_position( 150.f, 320.f );

		m_max_local_sequence_difference_value = text_manager->create_text( "" );
		m_max_local_sequence_difference_value.set_visible( true );
		m_max_local_sequence_difference_value.set_color( 0, 0xFF, 0xFF, 0xFF );
		m_max_local_sequence_difference_value.set_position( 325.f, 320.f );

		m_unacknowledged_packets_caption = text_manager->create_text( "unacknowledged :" );
		m_unacknowledged_packets_caption.set_visible( true );
		m_unacknowledged_packets_caption.set_color( 0xFF, 0xFF, 0, 0xFF );
		m_unacknowledged_packets_caption.set_position( 150.f, 340.f );

		m_unacknowledged_packets_value = text_manager->create_text( "" );
		m_unacknowledged_packets_value.set_visible( true );
		m_unacknowledged_packets_value.set_color( 0xFF, 0xFF, 0, 0xFF );
		m_unacknowledged_packets_value.set_position( 325.f, 340.f );
	}

	m_sent.set_visible( s_show_network_statistics );
	m_sent_low_level.set_visible( s_show_network_statistics );
	m_resent.set_visible( s_show_network_statistics );
	m_received.set_visible( s_show_network_statistics );
	m_received_low_level.set_visible( s_show_network_statistics );
	m_received_duplicated.set_visible( s_show_network_statistics );

	m_max_local_sequence_difference_caption.set_visible( s_show_network_statistics );
	m_max_local_sequence_difference_value.set_visible( s_show_network_statistics );
	m_unacknowledged_packets_caption.set_visible( s_show_network_statistics );
	m_unacknowledged_packets_value.set_visible( s_show_network_statistics );

	network_core::udp_match_stats const& stats = m_match_client.get_stats( );
	m_sent.set_text( current_time_in_ms, stats.sent, m_previous_stats.sent );
	m_sent_low_level.set_text( current_time_in_ms, stats.sent_low_level, m_previous_stats.sent_low_level );
	m_resent.set_text( current_time_in_ms, stats.resent, m_previous_stats.resent );
	m_received.set_text( current_time_in_ms, stats.received, m_previous_stats.received );
	m_received_low_level.set_text( current_time_in_ms, stats.received_low_level, m_previous_stats.received_low_level );
	m_received_duplicated.set_text( current_time_in_ms, stats.received_duplicated, m_previous_stats.received_duplicated );

	m_previous_stats = stats;
	network_core::udp_match_stats difference = stats - m_previous_stats;

	char text[ 256 ];
	vostok::sprintf< 256 >( text, "%3d packets", m_previous_stats.max_local_sequence_difference );
	m_max_local_sequence_difference_value.set_text( text );
	vostok::sprintf< 256 >( text, "%3d packets", m_previous_stats.unacknowledged_packets );
	m_unacknowledged_packets_value.set_text( text );
}

game_team_id network_client::get_player_team( pcstr player_profile_name )
{
	for ( u8 i = 0; i < 20; ++i )
		if ( strcmp( m_match_client.get_match_options( ).player_profiles[ i ].profile_name, player_profile_name ) == 0 )
			return m_match_client.get_match_options( ).player_profiles[ i ].team;

	return team_undefined;
}

} // namespace survarium
