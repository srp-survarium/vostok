// SPDX-License-Identifier: GPL-3.0-or-later

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

// claude@NOTE: target log line and severity values are fixed below. Remaining bytes
// are LTCG register selection at the two successful query call boundaries plus an
// ICF boost::function clear representative; target/base still share the same CFG.
// Revisit only if the caller compiler context or fold representative changes.
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
			if ( action == messaging::query_friend_list )
				read_friend_list( reader );
			else if ( action == messaging::update_friends_status )
				read_friend_status( reader );
			else if ( action == messaging::query_ignore_list )
				read_ignore_list( reader );
			else if ( action == messaging::find_players )
				read_found_players( reader );
			else
			{
				const u8 result = reader.r< u8 >( );
				if ( action == messaging::add_friend )
				{
					if ( result == '4' )	query_for_friend_list( );
#line 56
					else					LOG_INFO( "add_friend: operation denied " );
				}
				else if ( action == messaging::remove_friend )
				{
					if ( result == '4' )	query_for_friend_list( );
#line 66
					else					LOG_INFO( "remove_friend: operation denied " );
				}
				else if ( action == messaging::add_ignorable )
				{
					if ( result == '4' )	query_for_ignore_list( );
#line 76
					else					LOG_INFO( "add_ignorable: operation denied " );
				}
				else
				{
					if ( result == '4' )	query_for_ignore_list( );
#line 85
					else					LOG_INFO( "remove_ignorable: operation denied " );
				}
			}

			m_game.lobby_menu( ).on_friendship_status_recivied( action );
			break;
		}

		default:
#line 95
			LOG_ERROR( "messaging_client received unknown message:%d", message_type );
	}
}

// the localized prefixes are the Russian + English channel-name tags
// (\x043e\x0431\x0449\x0438\x0439 = general, etc.); spelled as code points so the
// emitted UTF-16 is codepage-independent
#line 82
messaging::message_channel_enum messaging_client::parse_receiver_channel( wchar_t const* w_receiver_name, const bool in_match )
{
	if ( w_receiver_name == wcsstr( w_receiver_name, L"\x043e\x0431\x0449\x0438\x0439" ) || w_receiver_name == wcsstr( w_receiver_name, L"general" ) )	return messaging::player_general_channel;

	if ( w_receiver_name == wcsstr( w_receiver_name, L"\x043e\x0442\x0440\x044f\x0434" ) || w_receiver_name == wcsstr( w_receiver_name, L"squad" ) )
		return messaging::player_squad_channel;

	if ( w_receiver_name == wcsstr( w_receiver_name, L"\x043a\x043b\x0430\x043d" ) || w_receiver_name == wcsstr( w_receiver_name, L"clan" ) )
		return messaging::player_clan_channel;

	if ( in_match ) {
		if ( w_receiver_name == wcsstr( w_receiver_name, L"\x0441\x0432\x043e\x0438\x043c" ) || w_receiver_name == wcsstr( w_receiver_name, L"team" ) )
			return m_game_team_id ? messaging::player_team2_channel : messaging::player_team1_channel;

		else if ( w_receiver_name == wcsstr( w_receiver_name, L"\x0432\x0441\x0435\x043c" ) || w_receiver_name == wcsstr( w_receiver_name, L"all" ) )
			return messaging::player_match_channel;
	}

	return messaging::player_private_channel;
}

void messaging_client::on_message_typed( wchar_t const* input_text, messaging::message_channel_enum message_chanel )
{
	wchar_t			w_receiver_name[32];
	bool			has_direct_receiver;
	wchar_t const*	receiver_end;

	has_direct_receiver = input_text[0] == L'/';
	receiver_end = wcsstr( input_text, L" " );
	if ( has_direct_receiver && receiver_end )
	{
		wcsncpy_s		( w_receiver_name, input_text + 1, receiver_end - input_text - 1 );

		message_chanel	= parse_receiver_channel( w_receiver_name, m_chat_handler.in_match( ) );
		input_text		= receiver_end + 1;
	}

	if ( message_chanel != messaging::player_private_channel )
		swprintf_s		( w_receiver_name, L"" );

	if ( m_connection_state == messaging::client_connected )
	{
		wchar_t w_sender_name[32];
		size_t converted_chars_count = 0;
		mbstowcs_s		( &converted_chars_count, w_sender_name, m_local_name, _TRUNCATE );

		m_chat_handler.add_message		( message_chanel, input_text, w_sender_name );
		if ( message_chanel == messaging::player_private_channel )
			m_chat_handler.add_to_recent_list	( w_receiver_name );

		char message_body[256];
		converted_chars_count = 0;
		errno_t const body_error = wcstombs_s( &converted_chars_count, message_body, input_text, _TRUNCATE );
		if ( body_error && body_error != STRUNCATE )
			strcpy_s	( message_body, "##text conversion error##" );

		char receiver_name[32];
		converted_chars_count = 0;
		errno_t const name_error = wcstombs_s( &converted_chars_count, receiver_name, w_receiver_name, _TRUNCATE );
		if ( name_error && name_error != STRUNCATE )
			strcpy_s	( message_body, "##name conversion error##" );

		u32 channel_id = 0;
		switch ( message_chanel )
		{
			case messaging::player_general_channel:
			case messaging::player_system_channel:
			case messaging::player_private_channel:
				break;

			case messaging::player_clan_channel:
				return;

			case messaging::player_match_channel:
				channel_id = m_match_channel_id_;
				if ( channel_id == u32( -1 ) )
					return;
				break;

			case messaging::player_team1_channel:
			case messaging::player_team2_channel:
				return;

			case messaging::player_squad_channel:
				channel_id = u32( -1 );
				if ( m_match_channel_id_ == u32( -1 ) )
					return;
				break;
		}

		network_core::tcp_packet	packet( memory::g_mt_allocator );
		packet.append	( ( u8 )0xC1 );
		packet.append	( channel_id );
		packet.append	( receiver_name );
		packet.append	( ( u8 )message_chanel );
		packet.append	( message_body );
		m_network_client.send	( packet );
	}
	else
		m_chat_handler.add_message		( messaging::player_system_channel, L"not connected to messaging server...", L"System" );
}

void messaging_client::query_for_friend_list( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_friend_list );
	m_network_client.send	( packet );
}

void messaging_client::query_for_friends_status( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::update_friends_status );
	m_network_client.send	( packet );
}

void messaging_client::add_to_friend_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::add_friend );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::remove_from_friend_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::remove_friend );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::query_for_ignore_list( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_ignore_list );
	m_network_client.send	( packet );
}

void messaging_client::add_to_ignore_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::add_ignorable );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::remove_from_ignore_list( const u32 account_id )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::remove_ignorable );
	packet.append		( account_id );
	m_network_client.send	( packet );
}

void messaging_client::find_players_by_name( pcstr player_name )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::find_players );
	packet.append		( player_name );
	m_network_client.send	( packet );
}

bool messaging_client::accept_message_from( const u32 sender_account_id, messaging::client_type_enum sender_type )
{
	bool const is_in_ignore_list =
		sender_type == messaging::account_client_type &&
		std::find( m_ignore_list.begin( ), m_ignore_list.end( ), sender_account_id ) != m_ignore_list.end( );

	return !is_in_ignore_list;
}

void messaging_client::process_incoming_text_message( network_core::packet_reader& reader )
{
	char						body[256];
	messaging::send_message_params	params;
	params.message_body		= &body;
	params.sender_type			= messaging::client_type_enum( reader.r< u8 >( ) );
	params.sender_account_id	= reader.r< u32 >( );

	if ( !accept_message_from( params.sender_account_id, params.sender_type ) )
		return;

	reader.r_string			( params.sender_name );
	params.message_channel	= messaging::message_channel_enum( reader.r< u8 >( ) );
	reader.r_string			( body );

	wchar_t w_sender_name[32];
	size_t converted_chars_count = 0;
	mbstowcs_s				( &converted_chars_count, w_sender_name, params.sender_name, _TRUNCATE );

	wchar_t w_text[1024];
	converted_chars_count = 0;
	mbstowcs_s				( &converted_chars_count, w_text, *params.message_body, _TRUNCATE );

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
