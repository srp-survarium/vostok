// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_SERVER_H_INCLUDED

#include <vostok/network_core/udp_match_client_session.h>
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/network_core/custom_alloc_handler.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/intrusive_list.h>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/intrusive/set.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace memory {
	class base_allocator;
} // namespace memory

namespace network_core {

class udp_match_packet;
extern memory::base_allocator* g_allocator;

// STATE[UNMATCHABLE]: the client PDB has type records but no emitted server web.
// sushi@TODO: verify the session/server/stat-dump source shape against a server target.
class udp_match_server : private boost::noncopyable {
public:
	struct comparer {
		inline	bool	operator()	( udp_match_client_session const& left, udp_match_client_session const& right ) const { return left.endpoint( ) < right.endpoint( ); }
		inline	bool	operator()	( boost::asio::ip::udp::endpoint const& left, udp_match_client_session const& right ) const { return left < right.endpoint( ); }
		inline	bool	operator()	( udp_match_client_session const& left, boost::asio::ip::udp::endpoint const& right ) const { return left.endpoint( ) < right; }
	}; // struct comparer

	struct destroy_predicate : private boost::noncopyable {
		inline	explicit	destroy_predicate	( udp_match_server& server ) : server( server ) { }
		inline	bool		operator()			( udp_match_client_session* session ) const
		{
			if ( !session->is_ready_to_be_destroyed( ) )
				return false;

			server.delete_client_impl	( session );
			return true;
		}
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
		m_packets_allocator		( packets_allocator ),
		m_time_in_ms				( 0 ),
		m_should_reopen_socket	( false ),
		m_is_accepting			( false )
	{
	}
	virtual									~udp_match_server			( )
	{
		if ( m_socket.is_open( ) )
			stop_accepting	( );

		while ( !m_clients.empty( ) ) {
			udp_match_client_session* session	= &*m_clients.begin( );
			m_clients.erase		( *session );
			session->instant_disconnect( );
			delete_client_impl	( session );
		}

		while ( !m_clients_to_destroy.empty( ) ) {
			udp_match_client_session* session	= m_clients_to_destroy.pop_front( );
			delete_client_impl	( session );
		}
	}

	inline	void							tick						( u32 time_in_ms )
	{
		m_time_in_ms	= time_in_ms;

		if ( m_should_reopen_socket )
			try_reopen_socket	( );

		if ( m_network_flow_emulator )
			m_network_flow_emulator->tick(
				time_in_ms,
				boost::bind( &udp_match_server::process_incoming_packet, this, _1, _2 )
			);

		typedef boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > clients_type;
		clients_type::iterator i	= m_clients.begin( );
		while ( i != m_clients.end( ) ) {
			udp_match_client_session* session	= &*i;
			++i;
			if ( session->is_disconnected( ) )
				delete_client	( session );
		}

		m_clients_to_destroy.remove_if	( destroy_predicate( *this ) );
		send_queued_packets				( time_in_ms );
	}

	inline	bool							are_there_any_queued_packets( ) const
	{
		typedef boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > clients_type;
		for ( clients_type::const_iterator i = m_clients.begin( ), e = m_clients.end( ); i != e; ++i )
			if ( i->are_there_any_queued_packets( ) )
				return true;

		return false;
	}

	inline	u32								clients_count				( ) const { return m_clients.size( ); }

	inline	udp_match_stats					get_stats					( ) const
	{
		udp_match_stats result;
		typedef boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > clients_type;
		for ( clients_type::const_iterator i = m_clients.begin( ), e = m_clients.end( ); i != e; ++i )
			result	+= i->get_stats( );

		if ( !m_clients.empty( ) )
			result	/= m_clients.size( );

		return result;
	}

	inline	boost::asio::io_service&		io_service					( ) { return m_socket.get_io_service(); }

	inline	void							set_on_packet_received		(
												boost::function< void( udp_match_client_session&, u8, packet_reader& ) > const&	callback
											) { m_on_packet_received = callback; }

	inline	void							start_accepting				( u16 port )
	{
		ASSERT							( UNKNOWN_EXPRESSION_T( !m_is_accepting ) );
		m_socket.open					( boost::asio::ip::udp::v4( ) );
		m_socket.bind					( boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4( ), port ) );
		m_is_accepting				= true;
		m_should_reopen_socket		= false;
		start_receiving				( );
	}
	inline	void							stop_accepting				( )
	{
		m_is_accepting				= false;
		m_should_reopen_socket		= false;
		if ( m_socket.is_open( ) ) {
			boost::system::error_code error_code;
			m_socket.cancel			( error_code );
			m_socket.close				( error_code );
		}
	}

	inline	void							enqueue						( udp_match_client_session& session, udp_match_packet* packet ) { session.enqueue( packet ); }

protected:
	virtual	void							delete_client				( udp_match_client_session*& session )
	{
		ASSERT							( UNKNOWN_EXPRESSION_T( session ) );
		m_clients.erase				( *session );
		session->instant_disconnect	( );
		m_clients_to_destroy.push_back( session );
		session							= NULL;
	}

private:
	inline	void							send_queued_packets			( u32 current_time_in_ms )
	{
		typedef boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > clients_type;
		for ( clients_type::iterator i = m_clients.begin( ), e = m_clients.end( ); i != e; ++i )
			i->send_queued_packets	( current_time_in_ms );
	}

	virtual	udp_match_client_session*		new_client					( boost::asio::ip::udp::endpoint const& endpoint )
	{
		return VOSTOK_NEW_IMPL( g_allocator, udp_match_client_session )(
			m_socket,
			endpoint,
			m_packets_allocator,
			m_packets_orderer,
			m_network_flow_emulator
		);
	}

	virtual	void							delete_client_impl			( udp_match_client_session*& session ) { VOSTOK_DELETE_IMPL( g_allocator, session ); }

	inline	void							process_incoming_packet		( packet_reader& reader, boost::asio::ip::udp::endpoint const& endpoint )
	{
		typedef boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > clients_type;
		clients_type::iterator i			= m_clients.find( endpoint, comparer( ) );
		udp_match_client_session* session	= i == m_clients.end( ) ? new_client( endpoint ) : &*i;
		if ( !session )
			return;

		if ( i == m_clients.end( ) )
			m_clients.insert	( *session );

		session->on_packet_received	( reader, m_on_packet_received );
	}

	inline	void							start_receiving				( )
	{
		m_socket.async_receive_from(
			boost::asio::buffer( m_receive_buffer ),
			m_remote_endpoint,
			make_custom_alloc_handler(
				m_handler_allocator,
				boost::bind( &udp_match_server::handle_receive, this, _1, _2 )
			)
		);
	}

	inline	u32								unacknowledged_packets_count( ) const
	{
		u32 result	= 0;
		typedef boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, &udp_match_client_session::set_member_hook >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > clients_type;
		for ( clients_type::const_iterator i = m_clients.begin( ), e = m_clients.end( ); i != e; ++i )
			result	+= i->unacknowledged_packets_count( );

		return result;
	}

	inline	void							handle_receive				( boost::system::error_code const& error_code, const u32 bytes_transferred )
	{
		if ( error_code ) {
			if ( error_code != boost::asio::error::operation_aborted ) {
				LOG_ERROR	( "error during reading from socket: %s\r\n", error_code.message( ).c_str( ) );
				on_error	( unable_to_read_from_socket, error_code );
			}
			return;
		}

		if ( !bytes_transferred ) {
			LOG_ERROR	( "unable to read from socket\r\n" );
			on_error	( unable_to_read_from_socket, error_code );
			return;
		}

		if ( m_network_flow_emulator )
			m_network_flow_emulator->on_packet_received(
				m_receive_buffer.c_array( ),
				bytes_transferred,
				m_remote_endpoint,
				m_time_in_ms,
				unacknowledged_packets_count( )
			);
		else {
			packet_reader reader( base_packet( m_receive_buffer.c_array( ), bytes_transferred ) );
			process_incoming_packet	( reader, m_remote_endpoint );
		}

		if ( m_is_accepting )
			start_receiving	( );
	}

	inline	void							on_error					( const client_error_codes_enum client_error_code, const boost::system::error_code error_code )
	{
		VOSTOK_UNREFERENCED_PARAMETERS	( client_error_code, error_code );
		m_should_reopen_socket	= m_is_accepting;
	}

	inline	bool							try_reopen_socket			( )
	{
		if ( !m_should_reopen_socket || !m_is_accepting )
			return false;

		boost::system::error_code error_code;
		boost::asio::ip::udp::endpoint const local_endpoint	= m_socket.local_endpoint( error_code );
		if ( error_code )
			return false;

		m_socket.close	( error_code );
		if ( error_code )
			return false;

		m_socket.open	( local_endpoint.protocol( ), error_code );
		if ( error_code )
			return false;

		m_socket.bind	( local_endpoint, error_code );
		if ( error_code )
			return false;

		m_should_reopen_socket	= false;
		start_receiving			( );
		return true;
	}

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
