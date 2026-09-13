// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_ASYNC_CONNECTOR_INLINE_H_INCLUDED
#define NETWORK_CORE_ASYNC_CONNECTOR_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline void async_connector::resolve( pcstr const host, const u32 port_number )
{
	LOG_INFO						( "host name is being resolved..." );
	ASSERT							( UNKNOWN_EXPRESSION_T( host ) );
	ASSERT							( UNKNOWN_EXPRESSION_T( m_connection_state == host_name_is_unresolved ) );
	m_connection_state				= host_name_is_being_resolved;

	boost::asio::ip::tcp::resolver* const resolver	= VOSTOK_NEW_IMPL( g_allocator, boost::asio::ip::tcp::resolver )( m_socket->get_io_service( ) );
	char port[ 6 ];
	_itoa_s							( port_number, port, 10 );
	boost::asio::ip::tcp::resolver::query query( host, port );
	resolver->async_resolve			(
		query,
		boost::bind( &async_connector::on_resolved, this, resolver, boost::asio::placeholders::error, boost::asio::placeholders::iterator )
	);
}

inline void async_connector::close_connection( )
{
	m_socket->shutdown				( boost::asio::socket_base::shutdown_both );
	m_socket->close					( );
}

inline void async_connector::on_error(
	const client_error_codes_enum	client_error_code,
	const boost::system::error_code	error_code
)
{
	m_connection_state	= host_name_is_unresolved;
	if ( m_on_error )
		m_on_error		( client_error_code, error_code );
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_ASYNC_CONNECTOR_INLINE_H_INCLUDED
