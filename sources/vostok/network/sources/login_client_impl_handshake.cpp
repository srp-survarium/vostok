////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// the login server conversation is partially protected by SSL:
//
// the client resolves the login server host and connects over
// a plain tcp socket (see login_client_impl_resolve.cpp and
// login_client_impl_connect.cpp), then writes the account name
// in the open and waits for the server answer
//
// if the server accepts the account name, the client initiates
// an SSL handshake over the very same socket (m_ssl_stream
// wraps a reference to m_socket) and only then transmits the
// password (and the sign up info, in case of a sign up) over
// the encrypted stream
//
// the handshake is verified against the login server
// certificate (see verify_ssl_certificate), so a failed
// verification results in cannot_handshake reported to the
// caller via the functor; the handshake is then retried
// retry_count times before giving up

// claude@NOTE: structure + locals match; residual = LOG-helper callback-ctor
// schedule in the three LOG blocks + the function1 copy lowering in the
// handshake( functor, ... ) retry call
void login_client_impl::on_handshaked(
		boost::system::error_code const&	error_code,
		boost::function< void ( handshaking_error_types_enum ) > const&	functor,
		u32									retry_count,
		const bool							stop_timer )
{
	ASSERT			( UNKNOWN_EXPRESSION_T( m_connection_state == handshaking ) );
	if ( error_code ) {
		m_connection_state	= connected;

		if ( retry_count ) {
			LOG_INFO	( "[LOGIN] NOT handshaked!\r\n" );
			LOG_ERROR	( "[LOGIN] error during handshaking: %s", error_code.message( ).c_str( ) );
			handshake	( functor, retry_count - 1, stop_timer );
		}
		else
			functor		( cannot_handshake );

		return;
	}

	LOG_INFO		( "[LOGIN] handshaked!\r\n" );
	m_connection_state	= handshaked;
	functor			( successfully_handshaked );
}
void login_client_impl::handshake( boost::function< void ( handshaking_error_types_enum ) > const& functor, const u32 retry_count, const bool stop_timer )
{
	if ( m_connection_state == handshaked ) {
		functor		( successfully_handshaked );
		return;
	}
	LOG_INFO		( "[LOGIN] handshaking...\r\n" );

	ASSERT			( UNKNOWN_EXPRESSION_T( m_connection_state == connected ) );
	m_connection_state	= handshaking;
	m_ssl_stream.async_handshake	(
		boost::asio::ssl::stream_base::client,
		boost::bind
		(
			&login_client_impl::on_handshaked,
			this,
			boost::asio::placeholders::error,
			functor,
			retry_count,
			stop_timer
		)
	);
}

// LOG_INFO must remain at target source line 64; its retained __LINE__ value
// controls the exact callback schedule (log-callback-ctor-schedule.md).

} // namespace network
} // namespace vostok
