// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/network_core/async_connector.h>
#include "network_core_memory.h"
namespace vostok {
namespace network_core {

async_connector::async_connector( ) :
	m_host				( ),
	m_socket			( NULL ),
	m_connection_state	( host_name_is_unresolved )
{
}

void async_connector::on_connected(
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
#line 23
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_connection_state == connection_is_being_established ) );
#line 26
	if ( error_code )
	{
#line 27
		m_connection_state	= host_name_is_unresolved;
		if ( m_on_error )
			m_on_error( server_cannot_be_connected, error_code );
		return;
	}
#line 33
	LOG_INFO( "connection_has_been_established!" );
	m_connection_state	= connection_has_been_established;

	if ( m_on_connected )
		m_on_connected( );
}
#line 35

void async_connector::connect( boost::asio::ip::tcp::resolver::iterator const& iterator )
#line 41
{
	m_connection_state	= connection_is_being_established;
	boost::asio::async_connect(
		*m_socket,
		iterator,
#line 52
		boost::bind( &async_connector::on_connected, this, _1, _2 ) );
}
#line 44

void async_connector::on_resolved(
	// the header declares this `* const` (target mangles QAV); the definition drops the
	// top-level const so DELETE( resolver ) can take the pointer by T*& and null it.
	boost::asio::ip::tcp::resolver*	resolver,
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
#line 60
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_connection_state == host_name_is_being_resolved ) );

	if ( error_code ) {
		LOG_INFO( "NOT host_name_has_been_resolved!" );
		LOG_INFO( "error during host_name_is_being_resolved: %s", error_code.message( ).c_str( ) );
		++iterator;
		if ( iterator != boost::asio::ip::tcp::resolver::iterator( ) ) {
			resolver->async_resolve(
				*iterator,
				boost::bind(
					&async_connector::on_resolved,
					this,
					resolver,
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator
				)
			);
			return;
		}

		DELETE( resolver );
		m_connection_state	= host_name_is_unresolved;
		LOG_INFO( "can't resolve endpoints: %s", error_code.message( ).c_str( ) );
		LOG_INFO( "please, try again later" );
		if ( m_on_error )
			m_on_error( host_cannot_be_resolved, error_code );
		return;
	}

	DELETE( resolver );
	LOG_INFO( "host name has been resolved!" );
	m_connection_state	= host_name_has_been_resolved;
	m_host				= iterator;
	connect( iterator );
}

void async_connector::connect(
	boost::asio::ip::tcp::socket&		socket,
	pcstr								host,
	u16									host_port,
	boost::function< void() > const&	on_connected,
	boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > const&	on_error
)
{
	m_socket			= &socket;
	m_connection_state	= host_name_is_unresolved;
	m_on_connected		= on_connected;
	m_on_error			= on_error;
#line 110
	LOG_INFO( "host name is being resolved..." );
#line 112
	ASSERT( UNKNOWN_EXPRESSION_T( host ) );
	m_connection_state	= host_name_is_being_resolved;
#line 115
	boost::asio::ip::tcp::resolver* const	resolver	= NEW( boost::asio::ip::tcp::resolver )( m_socket->get_io_service( ) );
	char	port[ 6 ];
#line 118
	_itoa_s( host_port, port, 10 );
#line 120
	boost::asio::ip::tcp::resolver::query	query( boost::asio::ip::tcp::v4( ), host, port );
	resolver->async_resolve(
		query,
#line 130
		boost::bind( &async_connector::on_resolved, this, resolver, boost::asio::placeholders::error, boost::asio::placeholders::iterator ) );
}

void async_connector::reset( )
{
	m_connection_state	= host_name_is_unresolved;
}

} // namespace network_core
} // namespace vostok
