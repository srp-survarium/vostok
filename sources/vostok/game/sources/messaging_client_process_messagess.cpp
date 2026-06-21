////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// the original compiland really is messaging_client_process_messagess.cpp
// (sic, PDB-attested typo) - keep the filename

#include "pch.h"
#include "messaging_client.h"

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>

#include "game.h"			// m_game.lobby_menu()
#include "lobby_menu.h"		// on_friendship_status_recivied / on_*_message_arrived
#include "chat_handler.h"	// m_chat_handler.add_message / add_to_recent_list
#include "messaging_enums.h"

namespace survarium {

// claude@NOTE: logging residual is the repo-wide wall - the target ICF-folds
// has_passed_filters/append onto empty_stub while the base emits the real logging
// bodies; structure/verbosity/strings are from the asm. The incoming message-type
// bytes (0xC9 text, 0xCC friendship) have no named constant in this trimmed tree.
void messaging_client::on_packet_received( network_core::packet_reader& reader )
{
	const u8 message_type = reader.r< u8 >( );
	switch ( message_type )
	{
		case 0xC9:
			process_incoming_text_message( reader );
			break;

		case 0xCC:
		{
			const messaging::friendship_actions_enum action = messaging::friendship_actions_enum( reader.r< u8 >( ) );
			if ( action == messaging::query_friend_list_action )
				read_friend_list( reader );
			else if ( action == messaging::query_friends_status_action )
				read_friend_status( reader );
			else if ( action == messaging::query_ignore_list_action )
				read_ignore_list( reader );
			else if ( action == messaging::find_players_action )
				read_found_players( reader );
			else
			{
				const u8 result = reader.r< u8 >( );
				if ( action == messaging::add_friend_action )
				{
					if ( result == '4' )	query_for_friend_list( );
					else					LOG_ERROR( "add_friend: operation denied" );
				}
				else if ( action == messaging::remove_friend_action )
				{
					if ( result == '4' )	query_for_friend_list( );
					else					LOG_ERROR( "remove_friend: operation denied" );
				}
				else if ( action == messaging::add_to_ignore_action )
				{
					if ( result == '4' )	query_for_ignore_list( );
					else					LOG_ERROR( "add_ignorable: operation denied" );
				}
				else
				{
					if ( result == '4' )	query_for_ignore_list( );
					else					LOG_ERROR( "remove_ignorable: operation denied" );
				}
			}

			m_game.lobby_menu( ).on_friendship_status_recivied( action );
			break;
		}

		default:
			LOG_ERROR( "messaging_client received unknown message %d", message_type );
	}
}

// the localized substrings are the Russian + English channel-name tags
// (\x043e\x0431\x0449\x0438\x0439 = general, etc.); spelled as code points so the
// emitted UTF-16 is codepage-independent
messaging::message_channel_enum messaging_client::parse_receiver_channel( wchar_t const* w_receiver_name, const bool in_match )
{
	if ( wcsstr( w_receiver_name, L"\x043e\x0431\x0449\x0438\x0439" ) || wcsstr( w_receiver_name, L"general" ) )	return messaging::player_general_channel;

	if ( wcsstr( w_receiver_name, L"\x043e\x0442\x0440\x044f\x0434" ) || wcsstr( w_receiver_name, L"squad" ) )
		return messaging::player_squad_channel;

	if ( wcsstr( w_receiver_name, L"\x043a\x043b\x0430\x043d" ) || wcsstr( w_receiver_name, L"clan" ) )
		return messaging::player_clan_channel;

	if ( in_match ) {
		if ( wcsstr( w_receiver_name, L"\x0441\x0432\x043e\x0438\x043c" ) || wcsstr( w_receiver_name, L"team" ) )
			return m_game_team_id ? messaging::player_team2_channel : messaging::player_team1_channel;

		if ( wcsstr( w_receiver_name, L"\x0432\x0441\x0435\x043c" ) || wcsstr( w_receiver_name, L"all" ) )
			return messaging::player_match_channel;
	}

	return messaging::player_private_channel;
}

// STATE[STUB]
// claude@NOTE: deps now all present (chat_handler.h exists) - parked as a large
// focused follow-up, NOT a symbol wall. Recovered body (33 stmts, 7 locals incl.
// tcp_packet): detect a direct receiver (wcsstr the leading L"/ " token, wcsncpy_s
// the name, sete has_direct_receiver), resolve the channel via
// parse_receiver_channel (or swprintf_s a blank receiver when channel==4), then if
// m_connection_state==client_connected mbstowcs_s the receiver name, echo locally
// via m_chat_handler.add_message + add_to_recent_list, wcstombs_s the body+name
// (strcpy_s "##text/name conversion error##" on failure), build a 0xC1 tcp_packet
// (append channel byte, then length-prefixed receiver_name and message_body),
// tcp_packet_client::send, and on the system/private channels run the
// "not connected to messaging server" LOG. The byte-exact tcp_packet append
// sequencing + the channel jump table make this a careful reconstruction.
void messaging_client::on_message_typed( wchar_t const* input_text, messaging::message_channel_enum message_chanel )
{
}

void messaging_client::query_for_friend_list( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_friend_list_action );
	m_network_client.send	( packet );
}

void messaging_client::query_for_friends_status( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_friends_status_action );
	m_network_client.send	( packet );
}

void messaging_client::add_to_friend_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::add_friend_action );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::remove_from_friend_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::remove_friend_action );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::query_for_ignore_list( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_ignore_list_action );
	m_network_client.send	( packet );
}

void messaging_client::add_to_ignore_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::add_to_ignore_action );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::remove_from_ignore_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::remove_from_ignore_action );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::find_players_by_name( pcstr player_name )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::find_players_action );
	packet.append		( player_name );
	m_network_client.send	( packet );
}

bool messaging_client::accept_message_from( const u32 sender_account_id, messaging::client_type_enum sender_type )
{
	if ( sender_type == messaging::account_client_type )
		return std::find( m_ignore_list.begin( ), m_ignore_list.end( ), sender_account_id ) == m_ignore_list.end( );

	return true;
}

void messaging_client::process_incoming_text_message( network_core::packet_reader& reader )
{
	messaging::send_message_params	params;
	params.sender_type			= messaging::client_type_enum( reader.r< u8 >( ) );
	params.sender_account_id	= reader.r< u32 >( );

	if ( !accept_message_from( params.sender_account_id, params.sender_type ) )
		return;

	reader.r_string			( params.sender_name );
	params.message_channel	= messaging::message_channel_enum( reader.r< u8 >( ) );
	reader.r_string			( params.message_body );

	wchar_t w_sender_name[32];
	mbstowcs_s				( NULL, w_sender_name, params.sender_name, _TRUNCATE );

	wchar_t w_text[1024];
	mbstowcs_s				( NULL, w_text, params.message_body, _TRUNCATE );

	if ( params.message_channel == messaging::player_team2_channel )
		m_game.lobby_menu( ).on_match_message_arrived( w_text );
	else if ( params.message_channel == messaging::player_squad_channel )
		m_game.lobby_menu( ).on_stats_message_arrived( w_text, w_sender_name, params.message_channel );
	else
	{
		m_chat_handler.add_message( params.message_channel, w_text, w_sender_name );
		m_chat_handler.add_to_recent_list( w_sender_name );
	}
}

} // namespace survarium
