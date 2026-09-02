// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED

#include <vostok/network_core/udp_match_client_session.h>
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/intrusive_list.h>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/intrusive/set.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class packet_reader;
class udp_match_packet;
class udp_network_flow_emulator;

// STATE[REMOVED] (every `/* no source */` member below): udp_match_server is the
// dedicated-server class - it has ZERO symbols in the shipped client target binary
// and no TU in scope instantiates it (only #included by network_core_entry_point.cpp,
// never constructed). So all its inline members are uninstantiated in both binaries;
// the empty shams are correct. Reconstruct only if a dedicated-server build is matched.
class udp_match_server : private boost::noncopyable {
public:
	struct comparer {
		inline	bool	operator()	( udp_match_client_session const& left, udp_match_client_session const& right ) const { VOSTOK_UNREFERENCED_PARAMETERS( left, right ); return false; }
		inline	bool	operator()	( boost::asio::ip::udp::endpoint const& left, udp_match_client_session const& right ) const { VOSTOK_UNREFERENCED_PARAMETERS( left, right ); return false; }
		inline	bool	operator()	( udp_match_client_session const& left, boost::asio::ip::udp::endpoint const& right ) const { VOSTOK_UNREFERENCED_PARAMETERS( left, right ); return false; }
	}; // struct comparer

	struct destroy_predicate : private boost::noncopyable {
		inline	explicit	destroy_predicate	( udp_match_server& server ) : server( server ) { /* no source */ }
		inline	bool		operator()			( udp_match_client_session* session ) const { return false; }
		inline				~destroy_predicate	( ) { /* no source */ }

	public:
		/* 0x0000 */	udp_match_server&		server;
	}; // struct destroy_predicate

	typedef	intrusive_list< udp_match_client_session, udp_match_client_session*, &udp_match_client_session::next_in_destroy_list, threading::single_threading_policy, size_policy, no_debug_policy >	udp_match_client_session_list;

public:
	inline									udp_match_server			(
												boost::asio::io_service&		io_service,
												memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
												udp_match_packets_orderer&		packets_orderer,
												udp_network_flow_emulator*		network_flow_emulator
											) :
		m_socket				( io_service ),
		m_packets_orderer		( packets_orderer ),
		m_network_flow_emulator	( network_flow_emulator ),
		m_packets_allocator		( packets_allocator )
	{
		/* no source */
	}
	virtual									~udp_match_server			( ) { /* no source */ }

	inline	void							tick						( u32 time_in_ms ) { /* no source */ }

	inline	bool							are_there_any_queued_packets( ) const { return false; }

	inline	u32								clients_count				( ) const { return 0; }

	inline	udp_match_stats					get_stats					( ) const { return udp_match_stats(); }

	inline	boost::asio::io_service&		io_service					( ) { return m_socket.get_io_service(); }

	inline	void							set_on_packet_received		(
												boost::function< void( udp_match_client_session&, u8, packet_reader& ) > const&	callback
											) { /* no source */ }

	inline	void							start_accepting				( u16 port ) { /* no source */ }
	inline	void							stop_accepting				( ) { /* no source */ }

	inline	void							enqueue						( udp_match_client_session& session, udp_match_packet* packet ) { /* no source */ }

protected:
	virtual	void							delete_client				( udp_match_client_session*& session ) { /* no source */ }

private:
	inline	void							send_queued_packets			( u32 current_time_in_ms ) { /* no source */ }

	virtual	udp_match_client_session*		new_client					( boost::asio::ip::udp::endpoint const& endpoint ) { return NULL; }

	virtual	void							delete_client_impl			( udp_match_client_session*& session ) { /* no source */ }

	inline	void							process_incoming_packet		( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint ) { /* no source */ }

	inline	void							start_receiving				( ) { /* no source */ }

	inline	u32								unacknowledged_packets_count( ) const { return 0; }

	inline	void							handle_receive				( boost::system::error_code const& error_code, const u32 bytes_transferred ) { /* no source */ }

	inline	void							on_error					( const client_error_codes_enum client_error_code, const boost::system::error_code error_code ) { /* no source */ }

	inline	bool							try_reopen_socket			( ) { return false; }

private:
	/* 0x0008 */	boost::function< void( udp_match_client_session&, u8, packet_reader& ) >	m_on_packet_received;

protected:
	/* 0x0028 */	boost::asio::ip::udp::socket		m_socket;

private:
	/* 0x006c */	boost::asio::ip::udp::endpoint		m_remote_endpoint;

protected:
	/* 0x0088 */	boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none >	m_clients;
	/* 0x009c */	udp_match_client_session_list		m_clients_to_destroy;
	/* 0x00ac */	udp_match_packets_orderer&			m_packets_orderer;
	/* 0x00b0 */	udp_network_flow_emulator* const	m_network_flow_emulator;
	/* 0x00b4 */	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	m_packets_allocator;
private:
	/* 0x00b8 */	boost::array< u8, 256 >				m_receive_buffer;
	/* 0x01b8 */	u32									m_time_in_ms;
	/* 0x01bc */	bool								m_should_reopen_socket;
	/* 0x01bd */	bool								m_is_accepting;
	/* 0x01c0 */	handler_allocator					m_handler_allocator;
}; // class udp_match_server

STATIC_SIZE_ASSERT(udp_match_server, 0x5C8);
STATIC_SIZE_ASSERT(udp_match_server::comparer, 0x1);
STATIC_SIZE_ASSERT(udp_match_server::destroy_predicate, 0x4);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED
