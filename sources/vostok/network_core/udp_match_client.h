////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CLIENT_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CLIENT_H_INCLUDED

#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/handler_allocator.h>
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

class udp_match_client : public boost::noncopyable {
public:
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

	inline	void								set_on_disconnect			( boost::function< void( disconnect_event_types_enum ) > const& value ) { /* no source */ }

	inline	udp_match_packet*					new_packet					( u8 message_type ) { return NULL; }
	inline	void								delete_packet				( udp_match_packet*& packet ) { /* no source */ }

	inline	bool								are_there_any_queued_packets( ) const { return false; }

	inline	udp_match_stats const&				get_stats					( ) const { return m_connection.get_stats(); }

	inline	boost::asio::ip::udp::endpoint		endpoint					( ) const { return m_server_endpoint; }

	inline	u32									last_send_time_in_ms		( ) const { return 0; }
	inline	u32									last_receive_time_in_ms		( ) const { return 0; }
	inline	u32									last_activity_time_in_ms	( ) const { return 0; }

	inline	bool								is_connected				( ) const { return false; }
	inline	bool								is_disconnected				( ) const { return false; }

	inline	void								set_on_packet_received		( boost::function< void( u8, packet_reader& ) > const& value ) { /* no source */ }

private:
			void								start_receiving				( );

			void								handle_receive				( boost::system::error_code const& error_code, u32 bytes_transferred );
	inline	void								handle_send					( boost::system::error_code const& error_code, u32 bytes_transferred ) { /* no source */ }

			void								on_error					( client_error_codes_enum client_error_code, boost::system::error_code error_code );

			void								process_incoming_packet		( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint );

	inline	void								send						( udp_match_packet const& packet ) { /* no source */ }

			void								on_disconnect				( disconnect_event_types_enum disconnect_type );

public:
	inline										~udp_match_client			( ) { /* no source */ }

	static	void								construct_packet			( udp_match_packets_orderer& packets_orderer, udp_match_packet& packet, u8 message_type ) { /* no source */ }

private:
	/* 0x0000 */	udp_match_connection				m_connection;
	/* 0x0530 */	boost::function< void( u8, packet_reader& ) >	m_on_packet_received;
	/* 0x0550 */	boost::function< void( disconnect_event_types_enum ) >	m_on_disconnect;
	/* 0x0570 */	timing::timer						m_timer;
	/* 0x0588 */	boost::asio::ip::udp::socket		m_socket;
	/* 0x05cc */	boost::asio::ip::udp::endpoint		m_server_endpoint;
	/* 0x05e8 */	boost::asio::ip::udp::endpoint		m_remote_endpoint;
	/* 0x0604 */	boost::asio::io_service&			m_io_service;
	/* 0x0608 */	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	m_packets_allocator;
	/* 0x060c */	udp_network_flow_emulator* const	m_network_flow_emulator;
	/* 0x0610 */	u32									m_time_in_ms;
	/* 0x0614 */	bool								m_is_receiving;
	/* 0x0615 */	boost::array< u8, 256 >				m_receive_buffer;
	/* 0x0718 */	handler_allocator					m_handler_allocator;
}; // class udp_match_client

STATIC_SIZE_ASSERT(udp_match_client, 0xB20);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CLIENT_H_INCLUDED
