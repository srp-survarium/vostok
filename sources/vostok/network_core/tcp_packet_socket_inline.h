////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/custom_alloc_handler.h>

namespace vostok {
namespace network_core {

// STATE[40%|PARTIAL]: control flow fully matched (operation_aborted return, size check, both error
// branches, success path). Residual is two unavoidable kinds, verified statement-by-statement:
//   (1) LOG_ERROR __FILE__/__LINE__ wall: target pushes "C:\survarium\sources\..." + line 0x1D/..,
//       base pushes "Z:\home\sheep\Projects\..." + line 0x23/.. - string bytes AND line numbers differ
//       by build environment (cannot be faked).
//   (2) inline-boundary: target calls tcp_packet::allocated_size() out-of-line; base inlines it to a
//       [packet+0Ch] field read (LTCG decision on a trivial inline accessor).
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
	// structure-diff (condensed): 18 aligned stmts, 6 SIZE (the two LOG_ERROR blocks + their
	//   if(m_on_error) bucketing), 1 QTY (L30 bucketing of the on_error machinery).
	// VERDICT: control flow fully matched; residual = __FILE__/__LINE__ wall + allocated_size inline-boundary.
}

// STATE[PARTIAL]: control flow matched (operation_aborted return, size check, header re-read recursion);
// residual is the LOG_ERROR __FILE__/__LINE__ wall (C:\survarium vs Z:\home build path + differing line nums)
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
	// structure-diff: ~20 aligned stmts, sizes match by position except the LOG_ERROR __FILE__/__LINE__
	//   block (218 vs 215, 31 vs 30) - line numbers differ by source layout, not a shape defect.
	// VERDICT: control flow matched (operation_aborted return, sizeof(T) check, header re-read recursion);
	//   residual = __FILE__/__LINE__ wall. Same for <u8> and <u16> instantiations.
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
	// structure-diff: 1/1 stmt fully aligned. VERDICT: DONE.
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
	// structure-diff (condensed): 5 aligned stmts, 4 SIZE (both LOG_ERROR blocks + their if(m_on_error)),
	//   1 QTY (L142 bucketing). delete_packet + both error branches match.
	// VERDICT: control flow matched; residual = __FILE__/__LINE__ wall on the two LOG_ERROR calls.
}

// STATE[55%|PARTIAL]: new_packet + write + on_packet_has_been_sent shape correct; clone( packet ) is the
// RIGHT call - but packet<T>::clone() is an empty /* no source */ stub in packet_inline.h, so it folds to
// nothing and the whole copy step is missing from base. Target inlines clone() -> clear()+append(buffer,size),
// calling the out-of-line packet<tcp_packet>::append(void const*,u32). Fixing this needs clone()'s body in
// packet_inline.h (another matcher's file, do-not-touch) - reported to orchestrator, not fixed here.
template < typename Socket >
inline void tcp_packet_socket< Socket >::send( tcp_packet const& packet )
{
	tcp_packet*	cloned_packet	= new_packet( );
	cloned_packet->clone( packet );

	boost::system::error_code	error_code;
	boost::asio::write( m_socket, buffer_to_send( *cloned_packet ), boost::asio::transfer_all( ), error_code );

	on_packet_has_been_sent( cloned_packet, error_code, cloned_packet->buffer_size( ) );
	// structure-diff (condensed): target 6 stmts / base 4 (quantity -2). Missing: clone()'s clear() (L155)
	//   + append(buffer,size) (L179) bodies, which are stubbed in packet_inline.h.
	// VERDICT: send source shape correct; the gap is packet<T>::clone() being a no-source stub (shared file) - REPORTED.
}

// STATE[95.5%|DONE]: NEW(tcp_packet)(m_packet_allocator); objdiff reports 0 (unit-pairing), bytes match
template < typename Socket >
inline tcp_packet* tcp_packet_socket< Socket >::new_packet( )
{
	return VOSTOK_NEW_IMPL( m_packet_allocator, tcp_packet )( m_packet_allocator );
	// structure-diff: 1/1 stmt fully aligned, bytes match. VERDICT: DONE (objdiff 95.5% is unit-pairing noise).
}

// STATE[99.81%|DONE]: error_code ec; m_socket.cancel(ec)
template < typename Socket >
inline void tcp_packet_socket< Socket >::stop_receiving( )
{
	boost::system::error_code	error_code;
	m_socket.cancel( error_code );
	// structure-diff (condensed): 2/2 stmts, only m_socket.cancel(ec) differs by SIZE (0x31 vs 0x3a).
	// VERDICT: DONE; residual is the inline-boundary of basic_socket::cancel(ec).
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
