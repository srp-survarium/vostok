////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "messaging_client.h"

#include "game_memory.h"	// g_allocator for the vectora<> buildability init
#include "game.h"			// game::network_client() -> login_client()
#include "base_network_client.h"
#include "chat_handler.h"	// m_chat_handler.add_message in on_error

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network/login_client.h>

#include <algorithm>		// std::find over the friend/ignore lists

namespace survarium {

// claude@NOTE: banked residuals across this TU (structure matches the target;
// the leftover bytes are non-steerable from here):
//  - packet_reader::r<T>()/r_string and tcp_packet::append/send: the target
//    whole-program-inlines them (small-POD copies collapse to direct mov), our
//    build emits out-of-line `call`s - a network_core inlining cap. Drives the
//    read_*() loop-body folds and the friend-status r<u16> standalone-stmt gap.
//  - boost::function default-construct / assign_to: same inline cap (set_on_*,
//    set_on_packet_received).
//  - logging::append (LOG_ERROR/LOG_WARNING) embeds __LINE__ + the demangled
//    signature; those only byte-match when this file's line layout matches the
//    shipped source exactly, which we do not have.
//  - ctor / connect statement-count (5-vs-6, 6-vs-7): /Od line-table folds
//    (connection_info-init + sprintf merge; the m_active test hoisted into the
//    struct-copy statement) - a compiler scheduling artifact, not source shape.

 messaging_client::messaging_client( game& g ) :
	m_game( g ),
	m_chat_handler( g.get_chat_handler( ) ),
	m_connection_state( messaging::client_disconnected ),
	m_network_client( g.get_network_world( ) ),
	m_match_channel_id_( ( u32 )-1 ),
	m_friend_list( g_allocator ),
	m_ignore_list( g_allocator ),
	m_found_players_list( g_allocator )
{
	m_scheduler_identifier.m_active	= false;

	m_connection_info.session_id			= 0;
	m_connection_info.port					= 0;
	m_connection_info.host[ 0 ]				= 0;
	m_connection_info.connection_error_count	= 0;
	m_connection_info.need_resolve			= true;
	sprintf_s( m_local_name, "local" );

	m_network_client.set_on_connected		( boost::bind( &messaging_client::on_connected, this ) );
	m_network_client.set_on_disconnected	( boost::bind( &messaging_client::on_disconnected, this ) );
	m_network_client.set_on_error			( boost::bind( &messaging_client::on_error, this, _1, _2 ) );
}

 messaging_client::~messaging_client( )
{
}

void messaging_client::assign_match_channel_order(
	const u32				match_id,
	const game_team_id		team_id
)
{
	if ( m_match_channel_id_ == match_id || match_id == ( u32 )-1 )
		return;

	m_match_channel_id_	= match_id;
	m_game_team_id		= team_id;
	update_channel_subscriptions	( );
}

void messaging_client::connect( server_connection_info const& connection_info )
{
	m_connection_info	= connection_info;
	// claude@NOTE: scheduler lives at the real game+0x380 (game-layout wall -
	// the trimmed game.h does not reproduce that offset)
	if ( m_scheduler_identifier.m_active )
		m_game.scheduler( ).unregister	( &m_scheduler_identifier );

	if ( m_connection_state != messaging::client_disconnected )
		disconnect		( );

	if ( strcmp( m_connection_info.host, "x" ) != 0 && m_connection_info.port != 0 )
		m_network_client.connect	( m_connection_info.host, m_connection_info.port );
}

void messaging_client::disconnect( )
{
	if ( m_connection_state == messaging::client_disconnected )
		return;

	m_connection_state	= messaging::client_disconnected;
	m_network_client.disconnect		( );
	m_network_client.set_on_packet_received	( boost::function< void ( network_core::packet_reader& ) >( ) );
}

void messaging_client::on_connected( )
{
	m_connection_state	= messaging::client_connecting;
	m_network_client.set_on_packet_received	(
		boost::bind( &messaging_client::sign_in_on_packet_received, this, _1 )
	);

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC3 );
	packet.append		( m_game.network_client( ).login_client( ).session_id( ) );
	packet.append		( ( u8 )5 );
	m_network_client.send	( packet );
}

void messaging_client::on_disconnected( )
{
	m_connection_state	= messaging::client_disconnected;
	m_network_client.set_on_packet_received	( boost::function< void ( network_core::packet_reader& ) >( ) );
}

void messaging_client::on_error(
	network_core::client_error_codes_enum	client_error_code,
	boost::system::error_code		system_error_code
)
{
	m_chat_handler.add_message	( messaging::player_system_channel, L"Lost connection to messaging server. Reconnecting...", L"System" );
	LOG_ERROR	( "%s", system_error_code.message( ).c_str( ) );
	disconnect		( );
	m_connection_info.connection_error_count++; m_connection_info.need_resolve = true;
}

bool messaging_client::read_friend_status( network_core::packet_reader& reader )
{
	const u32 status_count	= reader.r< u16 >( );
	for ( u32 i = 0; i < status_count; ++i ) {
		const u32 account_id	= reader.r< u32 >( );
		const bool online		= reader.r< bool >( );

		vectora< account_list_item >::iterator const found	=
			std::find( m_friend_list.begin( ), m_friend_list.end( ), account_id );
		if ( found != m_friend_list.end( ) )
			found->online	= online;
		else
			LOG_WARNING		( "Friend list out of sync" );
	}
	return true;
}

bool messaging_client::read_friend_list( network_core::packet_reader& reader )
{
	const u32 friend_count	= reader.r< u16 >( );
	m_friend_list.resize	( friend_count );
	for ( u32 i = 0; i < friend_count; ++i ) {
		account_list_item& item	= m_friend_list[ i ];
		item.account_id	= reader.r< u32 >( );
		reader.r_string	( item.account_name.get_buffer( ), ( u8 )item.account_name.get_buffer_size( ) );
		item.online	= reader.r< bool >( );
	}
	return true;
}

// claude@NOTE: read_ignore_list / read_found_players read only account_id + the
// name string per item (NO `online` field - unlike read_friend_list, confirmed by
// the target loop body: stride 0x34, stores at item+0x0 and item+0x10 only, no
// [item+0x30] online store). 7 stmts each, matches the target; byte residual is
// the r<T>/r_string inline wall noted at the top of the file.
bool messaging_client::read_ignore_list( network_core::packet_reader& reader )
{
	const u32 ignore_count	= reader.r< u16 >( );
	m_ignore_list.resize	( ignore_count );
	for ( u32 i = 0; i < ignore_count; ++i ) {
		account_list_item& item	= m_ignore_list[ i ];
		item.account_id	= reader.r< u32 >( );
		reader.r_string	( item.account_name.get_buffer( ), ( u8 )item.account_name.get_buffer_size( ) );
	}
	return true;
}

bool messaging_client::read_found_players( network_core::packet_reader& reader )
{
	const u32 found_count	= reader.r< u16 >( );
	m_found_players_list.resize	( found_count );
	for ( u32 i = 0; i < found_count; ++i ) {
		account_list_item& item	= m_found_players_list[ i ];
		item.account_id	= reader.r< u32 >( );
		reader.r_string	( item.account_name.get_buffer( ), ( u8 )item.account_name.get_buffer_size( ) );
	}
	return true;
}

} // namespace survarium
