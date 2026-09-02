// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// claude@NOTE: structure + locals match; residual = the LOG-helper callback-ctor
// schedule inside the two LOG_ERROR blocks (log-callback-ctor-schedule.md)
void login_client_impl::on_ping_sent(
		const u32							try_count,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred )
{

	if ( error_code ) {
		ping		( try_count - 1 );
		LOG_ERROR	( "[LOGIN] ping: error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR	( "[LOGIN] ping: unable to write to socket\r\n" );
		return;
	}

	m_ping_timer.expires_from_now	( boost::posix_time::seconds( 1 ) );
	m_ping_timer.async_wait			( boost::bind( &login_client_impl::ping, this, ping_retry_count ) );
}

void login_client_impl::ping( const u32 retry_count )
{
	if ( m_client_state != signed_in )
		return;

	if ( !retry_count ) {
		m_client_state	= signed_out;
		return;
	}

	m_ping_socket.async_send	(
		boost::asio::buffer( &m_session_id, sizeof( m_session_id ) ),
		boost::bind(
			&login_client_impl::on_ping_sent,
			this,
			retry_count,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

} // namespace network
} // namespace vostok
