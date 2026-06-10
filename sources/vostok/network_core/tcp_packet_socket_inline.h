////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED

#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/custom_alloc_handler.h>

namespace vostok {
namespace network_core {

// STATE[PARTIAL]: legacy flat early-return blocks (packet_socket_inline shape) - target's
// 18-stmt carcass line map (16..34 == legacy lines exactly) carries `delete_packet; return;`
// in BOTH error blocks, not an else-if chain. Residual is two unavoidable kinds:
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
		return;
	}

	if ( bytes_transferred != packet->allocated_size( ) )
	{
		LOG_ERROR( "unable to read from socket\r\n" );
		if ( m_on_error )
			m_on_error( unable_to_read_from_socket, error_code );

		delete_packet( packet );
		return;
	}

	if ( m_on_packet_received )
		m_on_packet_received( *packet );

	delete_packet( packet );

	start_receiving( );

	// STRUCTURE DIFF: target 18 stmts / base 18 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|t.ln|b.ln|b.code
	// ---------+--------+--------+----+----+----+----+------
	// SIZE -0x3|0x124fa8|0x0a26e8|0xbc|0xb9|+4  |+5  |LOG_ERROR( "error during reading from socket: %s\r\n", error_code.message( ).c_str( ) );
	// SIZE -0x1|0x125064|0x0a27a1|0x1f|0x1e|+5  |+6  |if ( m_on_error )
	// SIZE +0xa|0x1250db|0x0a2817|0x11|0x1b|+12 |+13 |if ( bytes_transferred != packet->allocated_size( ) )
	// SIZE -0x3|0x1250ec|0x0a2832|0x77|0x74|+13 |+15 |LOG_ERROR( "unable to read from socket\r\n" );
	// SIZE +0x1|0x125163|0x0a28a6|0x1e|0x1f|+14 |+16 |if ( m_on_error )
	// SIZE +0x1|0x1251d6|0x0a291a|0x1b|0x1c|+23 |+23 |if ( m_on_packet_received )
	// VERDICT: STRUCTURE MATCH (shape ok) - 18/18 after flat early-return adoption (was 17 + 1
	// ONLY-target); SIZE rows are the LOG_ERROR __FILE__/__LINE__ wall + allocated_size()
	// inline-boundary + regalloc, non-steerable.
}

// STATE[PARTIAL]: legacy tail adopted (packet_socket_inline shape): flat early-return error
// blocks, pointer-cast header-buffer deref, `if ( identity( sizeof(T) < sizeof(u16) ) )`
// template-recursion guard with else-ASSERT, early `return;` after the body-read async_read.
// Residual is the LOG_ERROR __FILE__/__LINE__ wall (C:\survarium vs Z:\home + line nums).
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
		return;
	}

	if ( bytes_transferred != sizeof( T ) )
	{
		LOG_ERROR( "unable to read from socket\r\n" );
		if ( m_on_error )
			m_on_error( unable_to_read_from_socket, error_code );
		return;
	}

	T const buffer_size	= *static_cast< T const* >( static_cast< pcvoid >( &m_header_buffer ) );
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
		return;
	}

	if ( identity( sizeof( T ) < sizeof( u16 ) ) )
		boost::asio::async_read(
			m_socket,
			boost::asio::buffer( &m_header_buffer, sizeof( u16 ) ),
			make_custom_alloc_handler(
				m_allocator,
				boost::bind( &tcp_packet_socket::on_packet_size_received< u16 >, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
			)
		);
	else
		ASSERT( sizeof( T ) < sizeof( u16 ) );

	// STRUCTURE DIFF: target 22 stmts / base 22 stmts (both <u8> 0x124480/0x97430 and
	// (<u8> instantiation, rva-pinned: target param demangles `const unsigned int` so identity join fails; <u16> mirrors it)
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|t.ln|b.ln|b.code
	// ----------+--------+--------+----+----+----+----+------
	// SIZE -0x3 |0x12451d|0x0a1c2d|0xda|0xd7|+4  |+5  |return;
	// SIZE -0x1 |0x1245f7|0x0a1d04|0x1f|0x1e|+5  |+6  |
	// SIZE -0x4 |0x12463f|0x0a1d4b|0x81|0x7d|+11 |+13 |if ( bytes_transferred != sizeof( T ) )
	// SIZE +0x46|0x124727|0x0a1e2f|0xd |0x53|+20 |+23 |{
	// SIZE +0x2b|0x124734|0x0a1e82|0xa5|0xd0|+35 |+32 |boost::bind( &tcp_packet_socket::on_packet_received, this, packet, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
	// SIZE +0x26|0x1247fb|0x0a1f74|0xbc|0xe2|+52 |+44 |boost::bind( &tcp_packet_socket::on_packet_size_received< u16 >, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )
	// VERDICT: STRUCTURE MATCH (shape ok) - 22/22 after the legacy tail adoption (was 20 vs
	// 24-row count): all three `return;` rows, the pointer-cast deref (0xf/0x11 exact), the
	// identity() guard (0x1d exact), the 2-byte `else` row and the 0x12-byte ASSERT eater all
	// reproduce; residual is the LOG_ERROR FILE/LINE wall + resize() inline-vs-call boundary.
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
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// SIZE +0x12|0x123fbf|0x0972cf|0x8c|0x9e|152   |	);
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE row is make_custom_alloc_handler's named-return
	// result-copy (6 movs) our LTCG fails to elide in this standalone emission (slot kept, frame 0xFC exact).
}

// STATE[PARTIAL]: legacy flat early-return blocks + `!bytes_transferred` spelling - target's
// 10-stmt carcass (lines == legacy+2) has `return;` rows in both blocks; residual is the
// LOG_ERROR __FILE__/__LINE__ wall only.
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
		return;
	}

	if ( !bytes_transferred )
	{
		LOG_ERROR( "unable to write to socket\n" );
		if ( m_on_error )
			m_on_error( unable_to_write_to_socket, error_code );
		return;
	}

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|t.ln|b.ln|b.code
	// ---------+--------+--------+----+----+----+----+------
	// SIZE -0x3|0x124108|0x0a1818|0xbf|0xbc|+3  |+4  |delete_packet( packet_being_sent );
	// SIZE -0x1|0x1241c7|0x0a18d4|0x1f|0x1e|+4  |+5  |
	// SIZE -0x4|0x12420f|0x0a191b|0x7b|0x77|+10 |+12 |}
	// VERDICT: STRUCTURE MATCH (shape ok) - 10/10 after flat early-return adoption (was 9 + 1
	// ONLY-target); SIZE rows are the LOG_ERROR __FILE__/__LINE__ wall, non-steerable.
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
