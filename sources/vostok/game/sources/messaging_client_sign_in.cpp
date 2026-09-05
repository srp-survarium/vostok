// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "messaging_client.h"
#include "chat_handler.h"

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

void messaging_client::sign_in_on_packet_received( network_core::packet_reader& reader )
{
	u8 const message_type	= reader.r< u8 >( );

	if ( message_type != 0xCB )
	{
		LOG_ERROR	( "messaging_client received unknown message:%d", message_type );
		return;
	}

	reader.r_string			( m_local_name );

	m_connection_info.connection_error_count	= 0;
	m_connection_state		= messaging::client_connected;
	m_network_client.set_on_packet_received	( boost::bind( &messaging_client::on_packet_received, this, _1 ) );

	LOG_INFO	( "Messaging client: signed in!" );

	m_chat_handler.set_local_player_name	( local_user_name( ) );
	m_chat_handler.add_message	( messaging::player_system_channel, L"Connected to messaging server.", L"System" );

	update_channel_subscriptions	( );
	query_for_friend_list	( );
	query_for_ignore_list	( );
}

void messaging_client::update_channel_subscriptions( )
{
	if ( !connected( ) )
		return;

	u32 channel_subscriptions[ messaging::max_channel_num ] = {
		0, ( u32 )-1, ( u32 )-1, 0, 0,
		m_match_channel_id_ != ( u32 )-1 ? m_match_channel_id_ : 0 };

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC5 );
	packet.append		( channel_subscriptions, ( u32 )sizeof( channel_subscriptions ) );
	m_network_client.send	( packet );
}

} // namespace survarium
