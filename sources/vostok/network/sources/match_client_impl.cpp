// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "match_client_impl.h"
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network {

// claude@NOTE: callback binding and assignment remain shared Boost context walls.
// claude@MATCH: array-size spelling preserves the target's runtime 0x2000 * 0x12c.
#line 24
match_client_impl::match_client_impl(
	boost::asio::io_service&	io_service,
	network_core::udp_match_packets_orderer&	packets_orderer,
	network_core::udp_network_flow_emulator_options const*	options
) :
	m_packets_allocator		( m_packets_storage.elems, m_packets_storage.size( ) * sizeof( m_packets_storage.elems[ 0 ] ) ),
	m_network_flow_emulator	( options ? NEW( network_core::udp_network_flow_emulator )( *g_allocator, m_packets_allocator, *options ) : 0 ),
	m_client				( io_service, m_packets_allocator, packets_orderer, m_network_flow_emulator ),
	m_state					( waiting_for_permission )
{
	m_client.set_on_packet_received	( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	m_client.set_on_disconnect		( boost::bind( &match_client_impl::on_disconnect, this, _1 ) );
}

// claude@NOTE: compiler-emitted member clearing differs through folded COMDAT ownership.
#line 38
match_client_impl::~match_client_impl( )
{
	VOSTOK_DELETE_IMPL		( g_allocator, m_network_flow_emulator );
}

// claude@NOTE: target retains the previous_state store but publishes no named local.
// Reader and callback access remain per-site inline-context walls.
// claude@MATCH: the return and block close share the target's source line.
#line 43
void match_client_impl::on_packet_received( const u8 message_type, network_core::packet_reader& reader )
{
	state const previous_state	= m_state;

	if ( message_type == set_status_ready_for_battle ) {
		ASSERT_U			( reader.eof( ) );
		m_state				= handshaked;
		m_client.set_on_packet_received( m_on_packet_received );

		if ( m_on_connected )
			m_on_connected	( successfully_connected, successfully_handshaked, no_socket_error, connection_successful );

		return; }
	LOG_ERROR				( "connection forbidden" );
	if ( m_on_connected )
		m_on_connected		(
			successfully_connected,
			successfully_handshaked,
			no_socket_error,
			invalid_session_id
		);

}

// claude@NOTE: m_on_connected assignment remains a shared Boost context wall.
#line 67
void match_client_impl::connect(
	pcstr const	host,
	const u16	port,
	const u32	current_time_in_ms,
	network_core::udp_match_packet const* const	packet,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	on_connected
)
{
	m_on_connected			= on_connected;
	m_client.connect		( host, port, packet ? clone_packet( *packet ) : 0, current_time_in_ms );
}

// claude@NOTE: callback assignment remains a shared Boost context wall.
#line 77
void match_client_impl::set_on_packet_received(
	boost::function< void ( u8, network_core::packet_reader& ) > const&	on_packet_received
)
{
	m_on_packet_received	= on_packet_received;
	if ( m_state == handshaked )
		m_client.set_on_packet_received( m_on_packet_received );
}

// claude@NOTE: packet accessors remain per-site inline-context walls; append differs at its call boundary.
#line 86
network_core::udp_match_packet* match_client_impl::clone_packet( network_core::udp_match_packet const& packet )
{
	network_core::udp_match_packet* const result	= network_core::new_udp_match_packet( m_packets_allocator );
	network_core::packet_reader	reader( packet );
	result->message_type	= packet.message_type;
	result->channel_id		= packet.channel_id;
	result->is_reliable		= packet.is_reliable;
	result->is_ordered		= packet.is_ordered;
	result->m_buffer[ 0 ]	= packet.m_buffer[ 0 ];
	result->append			( reader.pointer( ), reader.size_to_eof( ) );
	return					result;
}

// claude@NOTE: packet callback binding remains a shared Boost context wall.
#line 99
void match_client_impl::disconnect( )
{
	m_state					= waiting_for_permission;
	m_client.set_on_packet_received( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	m_client.disconnect		( );
}

// claude@NOTE: packet callback binding remains a shared Boost context wall.
#line 104
void match_client_impl::on_disconnect(
	const network_core::disconnect_event_types_enum	disconnect_type
)
{
	m_state					= waiting_for_permission;
	m_client.set_on_packet_received( boost::bind( &match_client_impl::on_packet_received, this, _1, _2 ) );
	if ( m_on_disconnect )
		m_on_disconnect		( disconnect_type );
}

} // namespace network
} // namespace vostok
