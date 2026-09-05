// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_MATCH_CLIENT_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CLIENT_H_INCLUDED

#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/custom_alloc_handler.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/network_core/disconnect_event_types_enum.h>
#include <vostok/timing_timer.h>
#include <boost/array.hpp>
#include <boost/function.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class packet_reader;
class udp_match_packet;
class udp_network_flow_emulator;
class process_packet_predicate;

class udp_match_client : private boost::noncopyable {
	// the predicate's operator() invokes the private m_on_packet_received directly.
	friend	class						process_packet_predicate;

public:
	typedef boost::function< void( u8, packet_reader& ) >	client_on_packet_received_type;
	typedef udp_match_connection							connection_type;

											udp_match_client			(
													boost::asio::io_service&		io_service,
													memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
													udp_match_packets_orderer&		packets_orderer,
													udp_network_flow_emulator*		network_flow_emulator
												);

			void								connect						(
													pcstr					host,
													u16						port,
													udp_match_packet*		packet,
													u32						current_time_in_ms
												);

			void								disconnect					( );

			void								enqueue						( udp_match_packet* packet );

			void								send_queued_packets			( u32 current_time_in_ms );

			void								check_consistency			( ) const;

	// STATE[INLINED]: body from match_client_impl ctor 0x766240 +0x1f7 (folded
	// function1::operator= into this+0x558 = m_on_disconnect)
	inline	void								set_on_disconnect			( boost::function< void( enum disconnect_event_types_enum ) > const& value ) { m_on_disconnect = value; }

	// STATE[UNMATCHABLE]: these legacy convenience seams mirror the connection API,
	// but the shipped wrapper allocates and constructs packets itself.
	inline	udp_match_packet*					new_packet					( const u8 message_type ) { return m_connection.new_packet( message_type ); }
	static	void							construct_packet			( udp_match_packets_orderer& packets_orderer, udp_match_packet& packet, u8 message_type ) { udp_match_connection::construct_packet( packets_orderer, packet, message_type ); }
	inline	void								delete_packet				( udp_match_packet*& packet ) { m_connection.delete_packet( packet ); }

	inline	bool								are_there_any_queued_packets( ) const { return m_connection.are_there_any_queued_packets( ); }

	inline	udp_match_stats const&				get_stats					( ) const { return m_connection.get_stats(); }

	inline	boost::asio::ip::udp::endpoint		endpoint					( ) const { return m_server_endpoint; }

	inline	u32									last_send_time_in_ms		( ) const { return m_connection.last_send_time_in_ms( ); }
	// STATE[INLINED]: body from match_client::last_receive_time_in_ms 0x74c5f0
	// (single load of impl+0x258134 = m_connection+0xfc)
	inline	u32									last_receive_time_in_ms		( ) const { return m_connection.last_receive_time_in_ms( ); }
	inline	u32									last_activity_time_in_ms	( ) const { return m_connection.last_activity_time_in_ms( ); }

	// STATE[INLINED]: bodies from match_client::is_connected/is_disconnected
	// 0x74c670/0x74c620 (cmp impl+0x258154 = m_connection.m_state vs 0/3)
	inline	bool								is_connected				( ) const { return m_connection.is_connected( ); }
	inline	bool								is_disconnected				( ) const { return m_connection.is_disconnected( ); }

	// STATE[INLINED]: body from match_client_impl 0x765d90 +0x73 / 0x765d30 +0x33
	// (folded function2::operator= into this+0x538 = m_on_packet_received)
	inline	void								set_on_packet_received		( client_on_packet_received_type const& value ) { m_on_packet_received = value; }

private:
			void								start_receiving				( );

			// const u32 here is load-bearing: __FUNCSIG__ in the LOG_ERROR sites renders from
			// the FIRST declaration, and the target literal reads "...,const unsigned int)".
			void								handle_receive				( boost::system::error_code const& error_code, const u32 bytes_transferred );
	inline	void								handle_send					( boost::system::error_code const& error_code, const u32 bytes_transferred )
	{
		if ( error_code ) {
			LOG_ERROR	( "error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
			on_error	( unable_to_write_to_socket, error_code );
			return;
		}

		if ( !bytes_transferred ) {
			LOG_ERROR	( "unable to write to socket\r\n" );
			on_error	( unable_to_write_to_socket, error_code );
		}
	}

			void								on_error					( const client_error_codes_enum client_error_code, const boost::system::error_code error_code );

			void								process_incoming_packet		( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint );

	inline	void								send						( udp_match_packet const& packet )
	{
		m_socket.async_send_to	(
			boost::asio::buffer( packet.buffer_to_send( ), packet.buffer_to_send_size( ) ),
			m_server_endpoint,
			make_custom_alloc_handler(
				m_handler_allocator,
				boost::bind( &udp_match_client::handle_send, this, _1, _2 )
			)
		);
	}

			void								on_disconnect				( const disconnect_event_types_enum disconnect_type );

private:
	typedef boost::array< u8, 256 >				receive_buffer_type;

	/* 0x0000 */	connection_type						m_connection;
	/* 0x0538 */	client_on_packet_received_type		m_on_packet_received;
	/* 0x0558 */	boost::function< void( enum disconnect_event_types_enum ) >	m_on_disconnect;
	/* 0x0578 */	timing::timer						m_timer;
	/* 0x0590 */	boost::asio::ip::udp::socket		m_socket;
	/* 0x05d4 */	boost::asio::ip::udp::endpoint		m_server_endpoint;
	/* 0x05f0 */	boost::asio::ip::udp::endpoint		m_remote_endpoint;
	/* 0x060c */	boost::asio::io_service&			m_io_service;
	/* 0x0610 */	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	m_packets_allocator;
	/* 0x0614 */	udp_network_flow_emulator* const	m_network_flow_emulator;
	/* 0x0618 */	u32									m_time_in_ms;
	/* 0x061c */	bool								m_is_receiving;
	/* 0x061d */	receive_buffer_type					m_receive_buffer;
	/* 0x0720 */	handler_allocator					m_handler_allocator;
}; // class udp_match_client

STATIC_SIZE_ASSERT(udp_match_client, 0xB28);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CLIENT_H_INCLUDED
