////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_CLIENT_SESSION_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_CLIENT_SESSION_H_INCLUDED

#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <boost/function.hpp>
#include <boost/intrusive/set_hook.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class packet_reader;
class udp_match_packet;
class udp_network_flow_emulator;

// STATE[REMOVED] (every `/* no source */` member below): udp_match_client_session is
// consumed only by udp_match_server - dedicated-server code with ZERO target
// symbols, never instantiated in scope. So all its inline members are uninstantiated in
// both binaries; the empty shams are correct. Reconstruct only if the server is matched.
class udp_match_client_session : private boost::noncopyable {
public:
	inline								udp_match_client_session	(
											boost::asio::ip::udp::socket&		socket,
											boost::asio::ip::udp::endpoint const&	endpoint,
											memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
											udp_match_packets_orderer&			packets_orderer,
											udp_network_flow_emulator*			flow_emulator
										) :
		// m_connection is non-default-constructible; the timeout/wait/idle/id args are
		// buildability placeholders (a matcher fills the real values).
		m_connection		( socket, endpoint, packets_allocator, packets_orderer, 0, 0, 0, NULL ),
		m_client_endpoint	( endpoint )
	{
		/* no source */
	}
	virtual								~udp_match_client_session	( ) { /* no source */ }

	virtual	void						on_packet_received			(
											packet_reader&		reader,
											boost::function< void( udp_match_client_session&, u8, packet_reader& ) > const&	callback
										) { /* no source */ }

	inline	void						enqueue						( udp_match_packet* packet ) { /* no source */ }

	inline	void						send_queued_packets			( u32 current_time_in_ms ) { /* no source */ }

	inline	void						instant_disconnect			( ) { /* no source */ }

	inline	udp_match_packet*			new_packet					( const u8 message_type ) { return NULL; }
	inline	void						delete_packet				( udp_match_packet*& packet ) { /* no source */ }

	inline	bool						are_there_any_queued_packets( ) const { return false; }

	inline	udp_match_stats const&		get_stats					( ) const { return m_connection.get_stats(); }

	inline	u32							unacknowledged_packets_count( ) const { return 0; }

	inline	boost::asio::ip::udp::endpoint const&	endpoint			( ) const { return m_client_endpoint; }

	inline	u32							last_send_time_in_ms		( ) const { return 0; }
	inline	u32							last_receive_time_in_ms		( ) const { return 0; }
	inline	u32							last_activity_time_in_ms	( ) const { return 0; }

	inline	bool						is_disconnected				( ) const { return false; }
	inline	bool						is_ready_to_be_destroyed	( ) const { return false; }

private:
	inline	void						on_error					( const client_error_codes_enum client_error_code, const boost::system::error_code error_code ) { /* no source */ }

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
