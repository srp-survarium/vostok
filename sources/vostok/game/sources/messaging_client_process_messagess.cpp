////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// the original compiland really is messaging_client_process_messagess.cpp
// (sic, PDB-attested typo) - keep the filename

#include "pch.h"
#include "messaging_client.h"

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[STUB]
// claude@NOTE: parked - lobby_menu wall. Recovered body is a switch on the
// leading message-type byte: case 0xC9 -> process_incoming_text_message(reader);
// case 0xCC -> a sub-switch on the next byte dispatching read_friend_list (5),
// read_friend_status (7), read_ignore_list (6), read_found_players (4), and the
// friend/ignore add/remove ops (0..3) which read an ack byte and either call
// query_for_friend_list / query_for_ignore_list (ack==0x34='4') or LOG_ERROR the
// "<op>: operation denied" message; default -> LOG_ERROR "received unknown". The
// 0xCC tail then notifies m_game.lobby_menu().on_friendship_status_recivied( op,
// found ) - lobby_menu is trimmed (lacks that method) and lives at game+0x374
// (game-layout wall). Unblock once lobby_menu is reconstructed.
void messaging_client::on_packet_received( network_core::packet_reader& reader )
{
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
// claude@NOTE: parked - chat_handler wall. Recovered body parses the receiver
// channel from input_text (wcsstr "/w "-style direct-receiver detection +
// parse_receiver_channel), formats the body, sends the typed message as a 0xC6
// packet (append channel, receiver_name, message_body), and echoes it locally via
// m_chat_handler.add_message(...) + add_to_recent_list(...). add_message/
// add_to_recent_list need chat_handler, which has no header in this tree
// (forward-declared only) - unresolved external at link. Unblock once
// chat_handler is reconstructed.
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

// STATE[STUB]
// claude@NOTE: parked - chat_handler / lobby_menu walls. Recovered body reads the
// message header (sender account id + type via accept_message_from, channel,
// sender name, body), widens the text (mbstowcs_s), and dispatches: match/stats
// channels -> m_game.lobby_menu().on_match_message_arrived / on_stats_message_
// arrived; otherwise m_chat_handler.add_message(...) + add_to_recent_list(...).
// chat_handler has no header and lobby_menu is trimmed (no on_*_arrived) - both
// unresolved at link. Unblock once chat_handler + lobby_menu are reconstructed.
void messaging_client::process_incoming_text_message( network_core::packet_reader& reader )
{
}

} // namespace survarium
