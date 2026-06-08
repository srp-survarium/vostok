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
}

// STATE[91%|PARTIAL]: body byte-correct; boost::function clear reps fold under LTCG
tcp_packet_client::~tcp_packet_client( )
{
	if ( m_async_connector.has_connection_established( ) )
		disconnect( );
}

// STATE[99.70%|DONE]: forwards to socket; residual is the inline-boundary of the called socket method
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
}

// STATE[99.69%|DONE]: forwards to socket; residual is the inline-boundary of the called socket method
void tcp_packet_client::send( tcp_packet const& packet )
{
	m_packet_socket.send( packet );
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
