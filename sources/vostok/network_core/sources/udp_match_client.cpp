// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/network_core/udp_match_client.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/process_packet_predicate.h>
#include <vostok/network_core/custom_alloc_handler.h>

namespace vostok {
namespace network_core {

udp_match_client::udp_match_client(
	boost::asio::io_service&			io_service,
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
	udp_match_packets_orderer&			packets_orderer,
	udp_network_flow_emulator* const	network_flow_emulator
) :
	m_connection		(
		m_socket,
		m_server_endpoint,
		packets_allocator,
		packets_orderer,
		120000,
		network_flow_emulator ? math::max( 250u, network_flow_emulator->max_ping_time_in_ms( ) * 6 ) : 500,
		33,
		"client"
	),
	m_socket			( io_service ),
	m_io_service		( io_service ),
	m_packets_allocator	( packets_allocator ),
	m_network_flow_emulator	( network_flow_emulator ),
	m_time_in_ms		( 0 ),
	m_is_receiving		( false )
{
	m_connection.set_on_disconnect( boost::bind( &udp_match_client::on_disconnect, this, _1 ) );
}
void udp_match_client::on_error( const client_error_codes_enum, const boost::system::error_code )
{
	m_connection.instant_disconnect( disconnected_by_connection_lost );
}
void udp_match_client::process_incoming_packet( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint )
{
	ASSERT_U( endpoint == m_server_endpoint );
	if ( m_network_flow_emulator && m_connection.is_disconnected( ) )
		return;

	ASSERT( UNKNOWN_EXPRESSION_T( !m_connection.is_disconnected( ) ) );
	m_connection.process_incoming_packet( reader, process_packet_predicate( *this ) );
}

void udp_match_client::handle_receive( boost::system::error_code const& error_code, const u32 bytes_transferred )
{
	check_consistency		( );

	ASSERT					( UNKNOWN_EXPRESSION_T( m_is_receiving ) );
	m_is_receiving			= false;

	if ( error_code ) {
		LOG_ERROR			(
			"error during reading from socket: %s\r\n",
			error_code.message( ).c_str( )
		);
		on_error			( unable_to_read_from_socket, error_code );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR			( "unable to read from socket\r\n" );
		on_error			( unable_to_read_from_socket, error_code );
		return;
	}

	if ( m_server_endpoint != m_remote_endpoint ) {
		LOG_ERROR			( "unexpected sender\r\n" );
		on_error			( unable_to_read_from_socket, error_code );
		return;
	}
	if ( m_network_flow_emulator ) {
		m_network_flow_emulator->on_packet_received( m_receive_buffer.c_array( ), bytes_transferred, m_remote_endpoint, m_time_in_ms, m_connection.unacknowledged_packets_count( ) );
	}
	else {
		packet_reader	reader( base_packet( m_receive_buffer.c_array( ), bytes_transferred ) );
		process_incoming_packet( reader, m_remote_endpoint );
	}
	check_consistency		( );
	if ( !m_connection.is_disconnected( ) )
		start_receiving		( );
}
void udp_match_client::start_receiving( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_is_receiving ) );
	m_is_receiving				= true;
	m_socket.async_receive_from	(
		boost::asio::buffer( m_receive_buffer ),
		m_remote_endpoint,
		make_custom_alloc_handler	(
			m_handler_allocator,
			boost::bind( &udp_match_client::handle_receive, this, _1, _2 )
		)
	);
}
void udp_match_client::connect(
	pcstr const					host,
	const u16					port,
	udp_match_packet* const		packet,
	const u32					current_time_in_ms
)
{
	if ( m_socket.is_open( ) )
		m_socket.close		( );
	m_socket.open			( boost::asio::ip::udp::v4( ) );
	m_socket.bind			( boost::asio::ip::udp::endpoint( ) );
	m_server_endpoint		= boost::asio::ip::udp::endpoint( boost::asio::ip::address::from_string( host ), port );
	m_connection.connect	( packet );
	check_consistency		( );
	start_receiving			( );
	m_connection.send_queued_packets( current_time_in_ms );
	check_consistency		( );
}
void udp_match_client::disconnect( )
{
	m_connection.disconnect( );
}
void udp_match_client::enqueue( udp_match_packet* packet )
{
	if ( m_connection.is_connected( ) ) {
		m_connection.enqueue		( packet );
	}
	else {
		LOG_ERROR					( "disconnection initiated but new packet has been enqueued" );
		m_connection.delete_packet	( packet );
	}
	check_consistency				( );
}

void udp_match_client::send_queued_packets( const u32 current_time_in_ms )
{
	if ( m_network_flow_emulator ) {
		// claude@MATCH: tick gets the OLD m_time_in_ms - the member is updated only after this block
		m_network_flow_emulator->tick( m_time_in_ms, boost::bind( &udp_match_client::process_incoming_packet, this, _1, _2 ) );
		if ( m_connection.is_disconnected( ) )
			return;
	}

	m_time_in_ms						= current_time_in_ms;
	check_consistency					( );
	m_connection.send_queued_packets	( current_time_in_ms );
	check_consistency					( );
}

void udp_match_client::check_consistency( ) const
{
	u32 const registered_packets_count	= m_packets_allocator.allocated_size( ) / sizeof( udp_match_packet );
	u32 const allocated_count			= ( m_network_flow_emulator ? m_network_flow_emulator->delayed_packets_count( ) : 0 ) + m_connection.packets_count( );
	ASSERT( UNKNOWN_EXPRESSION_T( registered_packets_count == allocated_count ) );
}

void udp_match_client::on_disconnect( const disconnect_event_types_enum disconnect_type )
{
	if ( m_is_receiving )
		m_socket.cancel( );

	if ( m_on_disconnect )
		m_on_disconnect( disconnect_type );
}


} // namespace network_core
} // namespace vostok
