////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/custom_alloc_handler.h>

namespace vostok {
namespace network_core {

// STATE[40%|PARTIAL]: control flow + operation_aborted early-return + allocated_size check matched;
// residual is the logging blocks (LOG_ERROR file-path string differs: C:\survarium vs our build path)
template < typename Socket >
inline void tcp_packet_socket< Socket >::on_packet_received(
	tcp_packet const*					packet,
	boost::system::error_code const&	error_code,
	u32									bytes_transferred
)
{
	if ( error_code )
	{
		if ( error_code == boost::asio::error::operation_aborted )
			return;

		LOG_ERROR( "error during reading from socket: %s\r\n", error_code.message( ).c_str( ) );
		if ( m_on_error )
			m_on_error( unable_to_read_from_socket, error_code );

		delete_packet( packet );
	} else if ( bytes_transferred != packet->allocated_size( ) )
	{
		LOG_ERROR( "unable to read from socket\r\n" );
		if ( m_on_error )
			m_on_error( unable_to_read_from_socket, error_code );

		delete_packet( packet );
	} else
	{
		if ( m_on_packet_received )
			m_on_packet_received( *packet );

		delete_packet( packet );

		start_receiving( );
	}
}

// STATE[PARTIAL]: control flow matched (operation_aborted return, size check, header re-read recursion);
// residual is the logging blocks (LOG_ERROR file-path string differs)
template < typename Socket >
template < typename T >
inline void tcp_packet_socket< Socket >::on_packet_size_received( boost::system::error_code const& error_code, u32 bytes_transferred )
{
	if ( error_code )
	{
		if ( error_code == boost::asio::error::operation_aborted )
			return;

		LOG_ERROR( "error during reading from socket: %s\r\n", error_code.message( ).c_str( ) );
		if ( m_on_error )
			m_on_error( unable_to_read_from_socket, error_code );
	} else if ( bytes_transferred != sizeof( T ) )
	{
		LOG_ERROR( "unable to read from socket\r\n" );
		if ( m_on_error )
			m_on_error( unable_to_read_from_socket, error_code );
	} else
	{
		const T	buffer_size	= static_cast< T >( m_header_buffer );
		if ( buffer_size )
		{
			tcp_packet* const	packet	= new_packet( );
			packet->resize( buffer_size );

			boost::asio::async_read(
				m_socket,
				buffer_to_receive_into( *packet ),
				make_custom_alloc_handler(
					m_allocator,
					boost::bind( &tcp_packet_socket::on_packet_received, this, packet, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
				)
			);
		} else
		{
			boost::asio::async_read(
				m_socket,
				boost::asio::buffer( &m_header_buffer, sizeof( u16 ) ),
				make_custom_alloc_handler(
					m_allocator,
					boost::bind( &tcp_packet_socket::on_packet_size_received< u16 >, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
				)
			);
		}
	}
}

// STATE[99.88%|DONE]: async_read header + custom_alloc_handler(bind on_packet_size_received<u8>)
template < typename Socket >
inline void tcp_packet_socket< Socket >::start_receiving( )
{
	boost::asio::async_read(
		m_socket,
		boost::asio::buffer( &m_header_buffer, sizeof( u8 ) ),
		make_custom_alloc_handler(
			m_allocator,
			boost::bind( &tcp_packet_socket::on_packet_size_received< u8 >, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
		)
	);
}

// STATE[52%|PARTIAL]: delete_packet + error/size branches matched; residual is logging file-path strings
template < typename Socket >
inline void tcp_packet_socket< Socket >::on_packet_has_been_sent(
	tcp_packet const*					packet_being_sent,
	boost::system::error_code const&	error_code,
	u32									bytes_transferred
)
{
	delete_packet( packet_being_sent );

	if ( error_code )
	{
		LOG_ERROR( "error during writing to socket: %s", error_code.message( ).c_str( ) );
		if ( m_on_error )
			m_on_error( unable_to_write_to_socket, error_code );
	} else if ( bytes_transferred == 0 )
	{
		LOG_ERROR( "unable to write to socket\n" );
		if ( m_on_error )
			m_on_error( unable_to_write_to_socket, error_code );
	}
}

// STATE[55%|PARTIAL]: new_packet + write + on_packet_has_been_sent shape; target does clear()+append(buffer,size)
// but base_packet::buffer() const is private (PDB-matched) so clone() is used here - take the hit
template < typename Socket >
inline void tcp_packet_socket< Socket >::send( tcp_packet const& packet )
{
	tcp_packet*	cloned_packet	= new_packet( );
	cloned_packet->clone( packet );

	boost::system::error_code	error_code;
	boost::asio::write( m_socket, buffer_to_send( *cloned_packet ), boost::asio::transfer_all( ), error_code );

	on_packet_has_been_sent( cloned_packet, error_code, cloned_packet->buffer_size( ) );
}

// STATE[95.5%|DONE]: NEW(tcp_packet)(m_packet_allocator); objdiff reports 0 (unit-pairing), bytes match
template < typename Socket >
inline tcp_packet* tcp_packet_socket< Socket >::new_packet( )
{
	return VOSTOK_NEW_IMPL( m_packet_allocator, tcp_packet )( m_packet_allocator );
}

// STATE[99.81%|DONE]: error_code ec; m_socket.cancel(ec)
template < typename Socket >
inline void tcp_packet_socket< Socket >::stop_receiving( )
{
	boost::system::error_code	error_code;
	m_socket.cancel( error_code );
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
