// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_MATCH_CLIENT_SESSION_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CLIENT_SESSION_H_INCLUDED

#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <boost/function.hpp>
#include <boost/intrusive/set_hook.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class packet_reader;
class udp_match_packet;
class udp_match_client_session : private boost::noncopyable {
public:
	inline								udp_match_client_session	(
											boost::asio::ip::udp::socket&		socket,
											boost::asio::ip::udp::endpoint const&	endpoint,
											memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
											udp_match_packets_orderer&			packets_orderer,
											udp_network_flow_emulator*			flow_emulator
										) :
		m_connection		(
			socket,
			endpoint,
			packets_allocator,
			packets_orderer,
			120000,
			flow_emulator ? math::max( 250u, flow_emulator->max_ping_time_in_ms( ) * 6 ) : 500,
			33,
			"server"
		),
		m_client_endpoint		( endpoint ),
		next_in_destroy_list	( NULL )
	{
		m_connection.connect	( NULL );
	}
	virtual								~udp_match_client_session	( ) { }

	virtual	void						on_packet_received			(
											packet_reader&		reader,
											boost::function< void( udp_match_client_session&, u8, packet_reader& ) > const&	callback
										)
	{
		m_connection.process_incoming_packet(
			reader,
			boost::bind( callback, boost::ref( *this ), _1, _2 )
		);
	}

	inline	void						enqueue						( udp_match_packet* packet ) { m_connection.enqueue( packet ); }

	inline	void						send_queued_packets			( u32 current_time_in_ms ) { m_connection.send_queued_packets( current_time_in_ms ); }

	inline	void						instant_disconnect			( ) { m_connection.instant_disconnect( disconnected_by_initiator ); }

	inline	udp_match_packet*			new_packet					( const u8 message_type ) { return m_connection.new_packet( message_type ); }
	inline	void						delete_packet				( udp_match_packet*& packet ) { m_connection.delete_packet( packet ); }

	inline	bool						are_there_any_queued_packets( ) const { return m_connection.are_there_any_queued_packets( ); }

	inline	udp_match_stats const&		get_stats					( ) const { return m_connection.get_stats(); }

	inline	u32							unacknowledged_packets_count( ) const { return m_connection.unacknowledged_packets_count( ); }

	inline	boost::asio::ip::udp::endpoint const&	endpoint			( ) const { return m_client_endpoint; }

	inline	u32							last_send_time_in_ms		( ) const { return m_connection.last_send_time_in_ms( ); }
	inline	u32							last_receive_time_in_ms		( ) const { return m_connection.last_receive_time_in_ms( ); }
	inline	u32							last_activity_time_in_ms	( ) const { return m_connection.last_activity_time_in_ms( ); }

	inline	bool						is_disconnected				( ) const { return m_connection.is_disconnected( ); }
	inline	bool						is_ready_to_be_destroyed	( ) const { return is_disconnected( ) && !m_connection.pending_operations_count( ); }

private:
	inline	void						on_error					( const client_error_codes_enum client_error_code, const boost::system::error_code error_code )
	{
		VOSTOK_UNREFERENCED_PARAMETERS	( client_error_code, error_code );
		m_connection.instant_disconnect	( disconnected_by_connection_lost );
	}

protected:
	/* 0x0008 */	udp_match_connection				m_connection;

private:
	/* 0x0540 */	boost::asio::ip::udp::endpoint		m_client_endpoint;

public:
	/* 0x055c */	boost::intrusive::set_member_hook<>	set_member_hook;
	/* 0x056c */	udp_match_client_session*			next_in_destroy_list;
}; // class udp_match_client_session

STATIC_SIZE_ASSERT(udp_match_client_session, 0x570);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_CLIENT_SESSION_H_INCLUDED
