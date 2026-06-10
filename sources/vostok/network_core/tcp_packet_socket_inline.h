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
	// STRUCTURE DIFF[target 0x124f20 | base 0x910e0]: target 18 / base 17 stmts
	// .. same ..
	//   4: 0x088 <0xbc> | 0x088 <0xb9> | LOG_ERROR( "error during reading from socket: %s\r\n", error_code.message( ).c_str( ) );   SIZE
	//   5: 0x144 <0x1f> | 0x141 <0x1e> | if ( m_on_error )   SIZE
	// .. same ..
	//   8: 0x1b6 <0x5> | 0x1b2 <0x20> | } else if ( bytes_transferred != packet->allocated_size( ) )   SIZE
	// .. same ..
	//   9: 0x1bb <0x11> | 0x1d2 <0x74> | LOG_ERROR( "unable to read from socket\r\n" );   SIZE
	//  10: 0x1cc <0x77> | 0x246 <0x1f> | if ( m_on_error )   SIZE
	//  11: 0x243 <0x1e> | --          | L30   ONLY target
	// .. same ..
	//  15: 0x2b6 <0x1b> | 0x2ba <0x1c> | if ( m_on_packet_received )   SIZE
	// .. same ..
	// ; aligned 11, size-diffs 6, quantity-diffs 1, blank-gaps 2
	// VERDICT: STRUCTURE MISMATCH (both) - all 3 branches + success path align; the 6 SIZE
	// diffs are the LOG_ERROR __FILE__/__LINE__ wall (C:\survarium vs Z:\home path bytes +
	// line nums) and allocated_size() inline-boundary; the 1 QTY (L30) is a if(m_on_error)
	// branch-bucketing split, not a source-shape miss. Unsteerable (env paths). trail: tcp_packet_socket_inline.md
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
	// STRUCTURE DIFF[<u8> target 0x124480 | base 0x906a0]: target 24 / base 20 stmts
	// .. same ..  (--view structure-diff cannot auto-align: target param demangles as
	//  `const unsigned int`, base as `unsigned int`, and a same-named vostok::network::
	//  packet_socket shadows the lookup - compared by pinned rva. Both share the error /
	//  size-check / success branches at matching offsets; the LOG_ERROR blocks carry the
	//  usual __FILE__/__LINE__ SIZE wall.)
	// ; target 24 / base 20 stmts -> quantity-diffs 4 (the success-path async_read + its
	//  recursion-guard rows split into more statements on target), size-diffs on LOG_ERROR
	// VERDICT: STRUCTURE MISMATCH (both) - control flow (operation_aborted return, sizeof(T)
	// check, header re-read recursion) matches; residual is the __FILE__/__LINE__ wall plus
	// the 4-stmt success-path async_read split (target keeps the buffer_to_receive_into /
	// bind rows as separate statements our LTCG folds). Same for <u8> and <u16>. trail: tcp_packet_socket_inline.md
}

// STATE[87.25%|PARTIAL]: frame now exact (0xFC) after the make_custom_alloc_handler named-return fix; residual is its result-copy (0x12 bytes) target LTCG elides in this standalone emission but keeps in the tcp_packet_client::start_reading emission (100% there - the spelling proof)
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
	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x12 | 146 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE row is make_custom_alloc_handler's named-return
	// result-copy (6 movs) our LTCG fails to elide in this standalone emission (slot kept, frame 0xFC exact).
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
	// STRUCTURE DIFF[target 0x1240b0 | base 0x902d0]: target 10 / base 9 stmts
	// .. same ..
	//   3: 0x058 <0xbf> | 0x058 <0xbc> | LOG_ERROR( "error during writing to socket: %s", error_code.message( ).c_str( ) );   SIZE
	//   4: 0x117 <0x1f> | 0x114 <0x1e> | if ( m_on_error )   SIZE
	// .. same ..
	//   6: 0x150 <0x5> | 0x14c <0xf> | } else if ( bytes_transferred == 0 )   SIZE
	// .. same ..
	//   7: 0x155 <0xa> | 0x15b <0x77> | LOG_ERROR( "unable to write to socket\n" );   SIZE
	//   8: 0x15f <0x7b> | --          | L142   ONLY target
	// .. same ..
	// ; aligned 5, size-diffs 4, quantity-diffs 1, blank-gaps 1
	// VERDICT: STRUCTURE MISMATCH (both) - delete_packet + both error branches align; the 4
	// SIZE diffs are the LOG_ERROR __FILE__/__LINE__ wall, the 1 QTY (L142) is if(m_on_error)
	// branch-bucketing. Not a source-shape miss; env-path unsteerable. trail: tcp_packet_socket_inline.md
}

// STATE[55%|PARTIAL]: new_packet/clone/write/on_packet_has_been_sent shape now correct -
// clone()'s body restored in packet_inline.h (m_buffer_size=0; append(other.buffer(),size)),
// so the copy step emits. Residual is the buffer()/buffer_size() accessor inline-boundary.
template < typename Socket >
inline void tcp_packet_socket< Socket >::send( tcp_packet const& packet )
{
	tcp_packet*	cloned_packet	= new_packet( );
	cloned_packet->clone( packet );

	boost::system::error_code	error_code;
	boost::asio::write( m_socket, buffer_to_send( *cloned_packet ), boost::asio::transfer_all( ), error_code );

	on_packet_has_been_sent( cloned_packet, error_code, cloned_packet->buffer_size( ) );
	// STRUCTURE DIFF[target 0x123ee0 | base 0x90170]: target 6 / base 5 stmts
	// .. same ..
	//   2: 0x028 <0x24> | 0x027 <0x2b> | cloned_packet->clone( packet );   SIZE
	// .. same ..
	//   3: 0x04c <0x16> | --          | L179   ONLY target
	// .. same ..
	//   5: --          | 0x061 <0x3a> | boost::asio::write( m_socket, buffer_to_send( *cloned_packet ), boost::asio::transfer_all( ), error_code );   ONLY base
	// .. same ..
	//   6: 0x071 <0x2d> | --          | L186   ONLY target
	// .. same ..
	// ; aligned 3, size-diffs 1, quantity-diffs 3, blank-gaps 0
	// VERDICT: STRUCTURE FIXED (was quantity 4 -> 3) - clone() body restored, so the copy
	// step now emits as a real statement (base 4 -> 5 stmts). Residual: target keeps
	// other.buffer()/buffer_size() as out-of-line calls; base inlines the trivial accessors
	// to direct [other+0]/[other+4] field reads (LTCG inline-boundary, no source lever).
	// trail: tcp_packet_socket_inline.md
}

// STATE[95.5%|DONE]: NEW(tcp_packet)(m_packet_allocator); objdiff reports 0 (unit-pairing), bytes match
template < typename Socket >
inline tcp_packet* tcp_packet_socket< Socket >::new_packet( )
{
	return VOSTOK_NEW_IMPL( m_packet_allocator, tcp_packet )( m_packet_allocator );
	// STRUCTURE DIFF[target 0x1242d0 | base 0x904f0]: target 1 / base 1 stmts
	// .. same ..
	// ; aligned 1, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 1/1 stmt aligned, quantity 0 / size 0; objdiff
	// 95.5% is template unit-pairing noise, bytes match. trail: tcp_packet_socket_inline.md
}

// STATE[99.81%|DONE]: error_code ec; m_socket.cancel(ec)
template < typename Socket >
inline void tcp_packet_socket< Socket >::stop_receiving( )
{
	boost::system::error_code	error_code;
	m_socket.cancel( error_code );
	// STRUCTURE DIFF[target 0x124050 | base 0x90270]: target 2 / base 2 stmts
	// .. same ..
	//   2: 0x01e <0x31> | 0x01e <0x3a> | m_socket.cancel( error_code );   SIZE
	// ; aligned 1, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 2/2 stmts align, quantity 0; sole SIZE is the
	// inline-boundary of basic_socket::cancel(ec). trail: tcp_packet_socket_inline.md
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
