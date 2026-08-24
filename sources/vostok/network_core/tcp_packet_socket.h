////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <boost/function.hpp>

#include <boost/asio.hpp>

namespace vostok {

namespace memory {
	class base_allocator;
} // namespace memory

namespace network_core {

class tcp_packet;

template < typename Socket >
class tcp_packet_socket : private core::noncopyable {
public:
	inline								tcp_packet_socket		( Socket& socket, memory::base_allocator& packet_allocator ) :
		m_packet_allocator	( packet_allocator ),
		m_socket			( socket ),
		m_header_buffer		( 0 )
	{
		/* no source */
	}

			void						send					( tcp_packet const& packet );

	inline	Socket&						next_layer				( ) const { return m_socket; }
	inline	Socket&						lowest_layer			( ) const { return m_socket; }

			void						start_receiving			( );
			void						stop_receiving			( );

	// STATE[REMOVED]: network_core::tcp_packet_client (the only tcp_packet_socket holder
	// in scope) drives the socket via start_receiving/stop_receiving/send and never wires
	// these callback setters; no other in-scope consumer instantiates the socket. Both sides.
	inline	void						set_on_packet_received	( boost::function< void( tcp_packet const& ) > const& value ) { /* no source */ } // STATE[REMOVED]
	inline	void						set_on_error			(
											boost::function< void( client_error_codes_enum, boost::system::error_code ) > const&	value
										) { /* no source */ } // STATE[REMOVED]

// claude@MATCH: target mangling is AAE (private) for the four members below -
// QAE (public) left them unpairable in objdiff (the unit's 0% block).
private:
			void						on_packet_received		(
											tcp_packet const*					packet,
											boost::system::error_code const&	error_code,
											u32									bytes_transferred
										);
			void						on_packet_has_been_sent	(
											tcp_packet const*					packet_being_sent,
											boost::system::error_code const&	error_code,
											u32									bytes_transferred
										);

	template < typename T >
			void						on_packet_size_received	(
											boost::system::error_code const&	error_code,
											u32 const							bytes_transferred
										);

			tcp_packet*					new_packet				( );
	inline	void						delete_packet			( tcp_packet const*& packet )
	{
		VOSTOK_DELETE_IMPL( m_packet_allocator, packet );
	}

public:
	inline								~tcp_packet_socket		( ) { /* no source */ }

private:
	/* 0x0000 */	boost::function< void( tcp_packet const& ) >								m_on_packet_received;
	/* 0x0020 */	boost::function< void( client_error_codes_enum, boost::system::error_code ) >	m_on_error;
	/* 0x0040 */	handler_allocator											m_allocator;
	/* 0x0448 */	memory::base_allocator&										m_packet_allocator;
	/* 0x044c */	Socket&														m_socket;
	/* 0x0450 */	u32															m_header_buffer;
}; // class tcp_packet_socket

STATIC_SIZE_ASSERT(tcp_packet_socket< boost::asio::ip::tcp::socket >, 0x458);

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/tcp_packet_socket_inline.h>

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED
