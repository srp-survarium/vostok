// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// claude@NOTE: structure + locals match; residual = LOG-helper callback-ctor
// schedule x4 (log-callback-ctor-schedule.md) + the function1 copy lowering at
// the connect( ) retry call
void login_client_impl::on_connected(
		const u32								retry_count,
		boost::function< void ( connection_error_types_enum ) > const&	functor,
		boost::asio::ip::tcp::resolver::iterator	start_iterator,
		boost::system::error_code const&		error_code,
		boost::asio::ip::tcp::resolver::iterator	iterator
	)
{

	ASSERT			( UNKNOWN_EXPRESSION_T( m_connection_state == connecting ) );

	if ( error_code ) {
		m_connection_state	= resolved;

		if ( retry_count ) {
			LOG_INFO	( "[LOGIN] NOT connected!\r\n" );
			LOG_ERROR	( "[LOGIN] error during connecting: %s\r\n", error_code.message( ).c_str( ) );
			LOG_INFO	( "[LOGIN] reconnecting...\r\n" );
			connect		( successfully_resolved, start_iterator, retry_count - 1, functor );
		}
		else {
			m_connection_state	= unresolved;
			functor		( cannot_connect );
		}

		return;
	}

	LOG_INFO		( "[LOGIN] connected!\r\n" );
	m_connection_state	= connected;
	functor			( successfully_connected );
}

// claude@NOTE: structure + locals match; residual = LOG-helper callback-ctor
// schedule x2 + the functor bind-copy lowering in the async_connect bind
void login_client_impl::connect(
		resolve_error_types_enum	error,
		boost::asio::ip::tcp::resolver::iterator	iterator,
		const u32					retry_count,
		boost::function< void ( connection_error_types_enum ) > const&	functor )
{
	ASSERT			( UNKNOWN_EXPRESSION_T( m_connection_state == resolved ) );

	if ( error == cannot_resolve ) {
		LOG_INFO	( "[LOGIN] NOT resolved!\r\n" );
		m_connection_state	= unresolved;
		return;
	}

	LOG_INFO		( "[LOGIN] connecting...\r\n" );
	strcpy_s		( m_host_ip, sizeof( m_host_ip ), iterator->endpoint( ).address( ).to_string( ).c_str( ) );

	m_connection_state	= connecting;
	boost::asio::async_connect	(
		m_socket,
		iterator,
		boost::bind(
			&login_client_impl::on_connected,
			this,
			retry_count,
			functor,
			iterator,
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator
		)
	);
}

} // namespace network
} // namespace vostok
