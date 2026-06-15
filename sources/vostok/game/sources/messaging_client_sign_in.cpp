////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "messaging_client.h"

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[STUB]
// claude@NOTE: parked - chat_handler wall. Recovered body: reads the sign-in
// message type byte; if it is 0xCB (signed in) it reads the local name into
// m_local_name, registers on_packet_received as the connected handler + sets
// m_connection_state = client_connected, LOG_INFO "Messaging client: signed in!",
// m_chat_handler.set_local_player_name(m_local_name), m_chat_handler.add_message(
// 2, L"Connected to messaging server.", L"System" ), then
// update_channel_subscriptions(); query_for_friend_list(); query_for_ignore_list();
// else LOG_WARNING "messaging_client received unknown ...". The
// set_local_player_name/add_message calls need chat_handler (no header in this
// tree) - unresolved at link. Unblock once chat_handler is reconstructed.
void messaging_client::sign_in_on_packet_received( network_core::packet_reader& reader )
{
}

void messaging_client::update_channel_subscriptions( )
{
	if ( m_connection_state != messaging::client_connected )
		return;

	u32 channel_subscriptions[ messaging::max_channel_num ] = {
		0,
		( u32 )-1,
		( u32 )-1,
		0,
		0,
		m_match_channel_id_ != ( u32 )-1 ? m_match_channel_id_ : 0,
		0,
		0,
		0,
	};

	network_core::tcp_packet	packet( memory::g_mt_allocator );
	packet.append		( ( u8 )0xC5 );
	packet.append		( channel_subscriptions, ( u32 )sizeof( channel_subscriptions ) );
	m_network_client.send	( packet );
}

} // namespace survarium
