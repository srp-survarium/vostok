// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/tcp_packet.h>
#include "network_core_memory.h"

namespace vostok {
namespace network_core {

tcp_packet_client::tcp_packet_client( boost::asio::io_service& io_service ) :
	m_socket		( io_service ),
	m_packet_socket	( m_socket, *g_allocator ),
	m_io_service	( io_service )
#line 19
{
	m_packet_socket.set_on_error( boost::bind( &tcp_packet_client::on_error, this, _1, _2 ) );
}

tcp_packet_client::~tcp_packet_client( )
{
	if ( !m_async_connector.has_connection_established( ) )
		return;

	disconnect( );
}

void tcp_packet_client::start_reading( )
{
	m_packet_socket.start_receiving( );
}

void tcp_packet_client::on_connected( )
{
	if ( m_on_connected )
		m_on_connected( );

	start_reading( );
}

void tcp_packet_client::connect( pcstr const host, u16 const port )
#line 45
{
	m_async_connector.connect(
		m_socket,
		host,
		port,
		boost::bind( &tcp_packet_client::on_connected, this ),
		m_on_error
#line 46
	);
}
#line 51

void tcp_packet_client::disconnect( )
#line 50
{
	m_packet_socket.stop_receiving( );

#line 52
	if ( m_socket.is_open( ) )
		close_connection( );
}
#line 59

void tcp_packet_client::close_connection( )
#line 57
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_socket.is_open( ) ) );

#line 59
	boost::system::error_code	ec;
	m_socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
	m_socket.close( );

#line 62
	m_async_connector.reset( );
}
#line 70

void tcp_packet_client::send( tcp_packet const& packet )
#line 66
{
	m_packet_socket.send( packet );
}

void tcp_packet_client::on_error( client_error_codes_enum client_error_code, boost::system::error_code error_code )
#line 71
{
	m_async_connector.reset( );

#line 73
	if ( m_on_error )
		m_on_error( client_error_code, error_code );
}
#line 83

} // namespace network_core
} // namespace vostok
