////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED

#include <boost/function.hpp>

#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/network_core/tcp_packet.h>

namespace vostok {
namespace network_core {

// claude@NOTE: the PDB shows tcp_packet_socket monomorphised on
// boost::asio::basic_stream_socket<ip::tcp,...> (i.e. boost::asio::ip::tcp::socket);
// reconstructed as the primary template it was in source. Bodies are stubs.
template < typename socket_type >
class tcp_packet_socket : public core::noncopyable {
public:
	inline						tcp_packet_socket			( socket_type& socket, memory::base_allocator& packet_allocator )
		: m_packet_allocator( packet_allocator ), m_socket( socket ), m_header_buffer( 0 ) { }

	inline						~tcp_packet_socket			( ) { }

			void				send						( tcp_packet const& packet );

	inline	socket_type&		next_layer					( ) const { return m_socket; }
	inline	socket_type&		lowest_layer				( ) const { return m_socket; }

			void				start_receiving				( );
			void				stop_receiving				( );

	inline	void				set_on_packet_received		( boost::function< void( tcp_packet const& ) > const& callback ) { m_on_packet_received = callback; }
	inline	void				set_on_error				( boost::function< void( client_error_codes_enum, boost::system::error_code ) > const& callback ) { m_on_error = callback; }

			void				on_packet_received			( tcp_packet const* packet, boost::system::error_code const& error_code, u32 bytes_transferred );

	template < typename size_type >
			void				on_packet_size_received		( boost::system::error_code const& error_code, u32 bytes_transferred );

			void				on_packet_has_been_sent		( tcp_packet const* packet_being_sent, boost::system::error_code const& error_code, u32 bytes_transferred );

			tcp_packet*			new_packet					( );
	inline	void				delete_packet				( tcp_packet const*& packet ) { VOSTOK_UNREFERENCED_PARAMETER( packet ); }

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	boost::function< void( tcp_packet const& ) >								m_on_packet_received;
	/* 0x0020 */	boost::function< void( client_error_codes_enum, boost::system::error_code ) >	m_on_error;
	/* 0x0040 */	handler_allocator											m_allocator;
	/* 0x0448 */	memory::base_allocator&										m_packet_allocator;
	/* 0x044c */	socket_type&												m_socket;
	/* 0x0450 */	u32															m_header_buffer;
}; // class tcp_packet_socket

STATIC_SIZE_ASSERT( tcp_packet_socket< boost::asio::ip::tcp::socket >, 0x458 );

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/tcp_packet_socket_inline.h>

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED
