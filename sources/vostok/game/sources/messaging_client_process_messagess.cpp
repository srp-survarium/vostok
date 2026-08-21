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

// claude@NOTE: the source branch structure, message values, and logging calls are
// target-attested. Gold tail-merges the repeated successful friend/ignore query
// paths while the base duplicates them; logging expansion then changes statement
// attribution. Revisit only with function-scoped compiler-context control.
void messaging_client::on_packet_received( network_core::packet_reader& reader )
#line 20
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
					else					LOG_ERROR( "add_friend: operation denied" );
				}
				else if ( action == messaging::remove_friend )
				{
					if ( result == '4' )	query_for_friend_list( );
					else					LOG_ERROR( "remove_friend: operation denied" );
				}
				else if ( action == messaging::add_ignorable )
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
#line 101
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

// claude@NOTE: structure-complete modulo three
// byte-capped residuals: (1) the packet<>::append/tcp_packet ctor/dtor + send chain is
// whole-program-inlined in gold but /Od here (the network template inline wall); the
// trailing inlined packet dtor offsets the append-row pairing. (2) the wcstombs_s error
// guard `(ret != 0 && ret != STRUNCATE)` emits TWO calls in /Od (no PDB local for the
// errno, so it is spelled inline) where gold CSEs to one. (3) input_text reassignment to
// the message-body pointer is scheduled early (line 132) by the optimizer; /Od keeps it
// after the wcsncpy that needs the original prefix, so it pairs one row off. None are
// structural divergences.
void messaging_client::on_message_typed( wchar_t const* input_text, messaging::message_channel_enum message_chanel )
#line 121
{
	wchar_t			w_receiver_name[32];
	bool			has_direct_receiver;

	if ( ( has_direct_receiver = ( input_text[0] == L'/' ) )
			&& wcsstr( input_text, L" " ) )
	{
		wcsncpy_s		( w_receiver_name, input_text + 1, ( wcsstr( input_text, L" " ) - input_text ) / 2 - 1 );

		message_chanel	= parse_receiver_channel( w_receiver_name, m_chat_handler.in_match( ) );
		input_text		= wcsstr( input_text, L" " ) + 1;
	}

	if ( message_chanel != messaging::player_private_channel )
		swprintf_s		( w_receiver_name, L"" );

	if ( m_connection_state == messaging::client_connected )
	{
		wchar_t w_sender_name[32];
		mbstowcs_s		( NULL, w_sender_name, m_local_name, _TRUNCATE );

		m_chat_handler.add_message		( message_chanel, input_text, w_sender_name );
		if ( message_chanel == messaging::player_private_channel )
			m_chat_handler.add_to_recent_list	( w_receiver_name );

		char message_body[256];
		if ( wcstombs_s( NULL, message_body, input_text, _TRUNCATE ) && wcstombs_s( NULL, message_body, input_text, _TRUNCATE ) != STRUNCATE )
			strcpy_s	( message_body, "##text conversion error##" );

		char receiver_name[32];
		if ( wcstombs_s( NULL, receiver_name, w_receiver_name, _TRUNCATE ) && wcstombs_s( NULL, receiver_name, w_receiver_name, _TRUNCATE ) != STRUNCATE )
			strcpy_s	( message_body, "##name conversion error##" );

		switch ( message_chanel )
		{
			case messaging::player_match_channel:
			case messaging::player_team1_channel:
			case messaging::player_team2_channel:
				if ( m_match_channel_id_ == u32( -1 ) )
					return;
				break;
		}

		network_core::tcp_packet	packet( memory::g_mt_allocator );
		packet.append	( ( u8 )0xC1 );
		packet.append	( m_match_channel_id_ );
		packet.append	( receiver_name );
		packet.append	( ( u8 )message_chanel );
		packet.append	( message_body );
		m_network_client.send	( packet );
	}
	else
		m_chat_handler.add_message		( messaging::player_system_channel, L"not connected to messaging server", L"System" );
}

void messaging_client::query_for_friend_list( )
#line 217
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_friend_list );
	m_network_client.send	( packet );
}

void messaging_client::query_for_friends_status( )
#line 228
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::update_friends_status );
	m_network_client.send	( packet );
}

void messaging_client::add_to_friend_list( const u32 account_id )
#line 239
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
#line 251
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
#line 263
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::query_ignore_list );
	m_network_client.send	( packet );
}

void messaging_client::add_to_ignore_list( const u32 account_id )
#line 274
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
#line 286
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
#line 298
{
	if ( m_connection_state != messaging::client_connected )
		return;

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC4 );
	packet.append		( ( u8 )messaging::find_players );
	packet.append		( player_name );
	m_network_client.send	( packet );
}

// claude@NOTE: anchor-capped. The target builds a single `bool` (set by the
// type==account && find!=end() test) and returns its negation through one unified
// epilogue (sete); our base compiles to 0 statements because the carcass anchor's
// freshly-built client has an empty m_ignore_list (begin==end), so LTCG const-folds
// the whole find away to `return true`. Lifts once a real client with a populated
// ignore list reaches this and the anchor is removed.
bool messaging_client::accept_message_from( const u32 sender_account_id, messaging::client_type_enum sender_type )
#line 310
{
	if ( sender_type == messaging::account_client_type )
		return std::find( m_ignore_list.begin( ), m_ignore_list.end( ), sender_account_id ) == m_ignore_list.end( );

	return true;
}

// claude@NOTE: structure-near-match (14/17). Two residuals: (1) the target INLINES
// accept_message_from here (the stlp __find expands in-line) where our base keeps the
// call - an inline-vs-call wall, not a source shape to change; (2) the target records a
// 4th local `body[256]` (a message_body staging/reference the gold build materialises
// from the inlined r_string path) that the /Od base does not. Both trace to the
// whole-program inlining of accept_message_from / packet_reader::r_string.
void messaging_client::process_incoming_text_message( network_core::packet_reader& reader )
#line 317
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
