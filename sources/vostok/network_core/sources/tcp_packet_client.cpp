////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/tcp_packet.h>
#include "network_core_memory.h"

namespace vostok {
namespace network_core {

// STATE[70%|PARTIAL]: body byte-correct; boost::bind/function assign reps fold under LTCG (frame/slot residual)
tcp_packet_client::tcp_packet_client( boost::asio::io_service& io_service ) :
	m_socket		( io_service ),
	m_packet_socket	( m_socket, *g_allocator ),
	m_io_service	( io_service ),
	m_first_packet	( NULL )
{
	m_on_error	= boost::bind( &tcp_packet_client::on_error, this, _1, _2 );
	// structure-diff (condensed): 1/1 stmt, SIZE 0xc4 (target) vs 0x9d (base).
	//   Member-init order matches. Residual: target INLINES the m_packet_socket(m_socket,*g_allocator)
	//   ctor (handler_allocator + boost::function field ctors emitted inline) where base emits one
	//   call tcp_packet_socket::tcp_packet_socket; plus the boost::function assign/swap epilogue
	//   register shape (esi/edi, frame 0x60 vs 0x6c) differs.
	// VERDICT: source shape correct; residual is LTCG inline-boundary on the member-socket ctor - take the hit.
}

// STATE[91%|PARTIAL]: body byte-correct; boost::function clear reps fold under LTCG
tcp_packet_client::~tcp_packet_client( )
{
	if ( m_async_connector.has_connection_established( ) )
		disconnect( );
	// structure-diff (condensed): target 3 stmts / base 2 (quantity +1 at L26, size 0x2).
	//   has_connection_established == 4 test matches; residual is the if branch layout:
	//   target emits `jne .disconnect; jmp .end` (the extra L26 stmt), base emits a single `je .end`.
	//   The trailing boost::function::clear runs are positional matches (template names fold under unit-pairing).
	// VERDICT: source shape correct; residual is LTCG if-branch layout (jne+jmp vs je) - no source lever, take the hit.
}

// STATE[99.70%|DONE]: forwards to socket; residual is the inline-boundary of the called socket method
void tcp_packet_client::start_reading( )
{
	m_packet_socket.start_receiving( );
	// structure-diff: 1/1 stmt fully aligned. VERDICT: DONE; residual is sub-statement inline boundary of start_receiving().
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
	// structure-diff (condensed): 5/5 stmts, only stmt4 differs by SIZE
	//   4: m_socket.close( ec )  target 0x0b (mov ecx; call basic_socket::close)
	//      base 0x28 (inlines basic_socket::close -> direct win_iocp_socket_service_base::close)
	// VERDICT: source shape correct; residual is a boost inline-boundary on close(ec) (no source lever) - take the hit.
}

// STATE[99.69%|DONE]: forwards to socket; residual is the inline-boundary of the called socket method
void tcp_packet_client::send( tcp_packet const& packet )
{
	m_packet_socket.send( packet );
	// structure-diff: 1/1 stmt fully aligned. VERDICT: DONE; residual is sub-statement inline boundary of socket send().
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
