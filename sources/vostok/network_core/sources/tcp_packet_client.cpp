////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/tcp_packet.h>
#include "network_core_memory.h"

namespace vostok {
namespace network_core {

// STATE[70.24%|PARTIAL]: body byte-correct; boost::bind/function assign reps fold under LTCG (frame/slot residual)
tcp_packet_client::tcp_packet_client( boost::asio::io_service& io_service ) :
	m_socket		( io_service ),
	m_packet_socket	( m_socket, *g_allocator ),
	m_io_service	( io_service ),
	m_first_packet	( NULL )
{
	m_on_error	= boost::bind( &tcp_packet_client::on_error, this, _1, _2 );

	// STRUCTURE DIFF[target 0x77cc70 | base 0x57b020]: target 1 / base 1 stmts
	//   1: 0x0c4 <0x5e> | 0x09d <0x67> | m_on_error	= boost::bind( &tcp_packet_client::on_error, this, _1, _2 );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - member-init order matches; sole SIZE is the
	// member-socket ctor inline-boundary (target inlines m_packet_socket ctor; base emits
	// one call) + boost::function assign epilogue regalloc. No source lever. trail: tcp_packet_client.md
}

// STATE[90.98%|PARTIAL]: body byte-correct; boost::function clear reps fold under LTCG
tcp_packet_client::~tcp_packet_client( )
{
	if ( m_async_connector.has_connection_established( ) )
		disconnect( );

	// STRUCTURE DIFF[target 0x77cbe0 | base 0x57af90]: target 3 / base 2 stmts
	// .. same ..
	//   2: 0x020 <0x2> | --          | L26   ONLY target
	// .. same ..
	// ; aligned 2, size-diffs 0, quantity-diffs 1, blank-gaps 1
	// VERDICT: STRUCTURE MISMATCH (quantity) - the 1 extra target stmt is a 2-byte branch
	// row: target lowers the guard as `jne .disconnect; jmp .end`, base as a single `je .end`.
	// has_connection_established test + disconnect call align; this is if-branch codegen layout,
	// not a source-shape miss - no source lever, take the hit. trail: tcp_packet_client.md
}

// STATE[100%|DONE]
void tcp_packet_client::start_reading( )
{
	m_packet_socket.start_receiving( );
}

// STATE[100%|DONE]
void tcp_packet_client::on_connected( )
{
	if ( m_on_connected )
		m_on_connected( );

	start_reading( );
}

// STATE[100%|DONE]
void tcp_packet_client::connect( pcstr host, u16 port )
{
	m_async_connector.connect(
		m_socket,
		host,
		port,
		boost::bind( &tcp_packet_client::on_connected, this ),
		m_on_error
	);
}

// STATE[100%|DONE]
void tcp_packet_client::disconnect( )
{
	m_packet_socket.stop_receiving( );

	if ( m_socket.is_open( ) )
		close_connection( );
}

// STATE[66%|PARTIAL]: ASSERT + shutdown + close + reset shape correct; residual = m_socket.close(ec)
// out-lines to basic_socket::close in target but over-inlines to win_iocp service close in base (inline-boundary)
void tcp_packet_client::close_connection( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_socket.is_open( ) ) );

	boost::system::error_code	ec;
	m_socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
	m_socket.close( ec );

	m_async_connector.reset( );

	// STRUCTURE DIFF[target 0x77cb10 | base 0x57aea0]: target 5 / base 5 stmts
	// .. same ..
	//   4: 0x059 <0xb> | 0x059 <0x28> | m_socket.close( ec );   SIZE
	// .. same ..
	// ; aligned 4, size-diffs 1, quantity-diffs 0, blank-gaps 2
	// VERDICT: STRUCTURE MATCH (shape ok) - 5/5 stmts align, quantity 0; sole SIZE is
	// m_socket.close(ec): target out-lines to basic_socket::close, base over-inlines to
	// win_iocp_socket_service_base::close - boost inline-boundary, no source lever. trail: tcp_packet_client.md
}

// STATE[99.69%|DONE]: forwards to socket; residual is the inline-boundary of the called socket method
void tcp_packet_client::send( tcp_packet const& packet )
{
	m_packet_socket.send( packet );

	// STRUCTURE DIFF[target 0x77cda0 | base 0x57ade0]: target 1 / base 1 stmts
	// .. same ..
	// ; aligned 1, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 1/1 stmt aligned, quantity 0 / size 0; residual
	// is the sub-statement inline boundary of socket send(). trail: tcp_packet_client.md
}

// STATE[100%|DONE]
void tcp_packet_client::on_error( client_error_codes_enum client_error_code, boost::system::error_code error_code )
{
	m_async_connector.reset( );

	if ( m_on_error )
		m_on_error( client_error_code, error_code );
}

} // namespace network_core
} // namespace vostok
