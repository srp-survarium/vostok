////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"
#include <vostok/login_server/login_structures.h>

namespace vostok {
namespace network {

login_client_impl::login_client_impl( boost::asio::io_service& io_service ) :
	m_socket			( io_service ),
	m_ssl_context		( boost::asio::ssl::context::sslv23 ),
	m_ssl_stream		( m_socket, m_ssl_context ),
	m_ping_socket		(
		io_service,
		boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4( ), 0 )
	),
	m_ping_timer		( io_service, boost::posix_time::seconds( 1 ) ),
	m_io_service		( io_service ),
	m_client_state		( signed_out ),
	m_connection_state	( unresolved ),
	m_session_id		( 0 ),
	m_host_port			( 0xffff ),
	m_in_destructor		( false )
{

	m_server_browser_address[0]			= 0;
	m_server_browser_initial_query[0]	= 0;

	m_ssl_context.load_verify_file		(
		"../../resources/ssl/survarium_login_server.crt" );
	m_ssl_stream.set_verify_mode		( boost::asio::ssl::verify_peer );
	m_ssl_stream.set_verify_callback	(
		boost::bind(
			&login_client_impl::verify_ssl_certificate,
			this,
			_1,
			_2
		)
	);

	m_host[0]		= 0;
	m_host_ip[0]	= 0;
}

// claude@NOTE: structure + locals match (do/switch with the folded case-breaks
// verified); residual = +0x3 at each sign_out( boost::function<...>( ) ) temp -
// the folded function4 default-ctor takes `this` via a reg the base reloads
login_client_impl::~login_client_impl( )
{
	m_in_destructor		= true;

	do {
		switch ( m_connection_state ) {
			case unresolved :		return;
			case resolving :		break;
			case connected :
				switch ( m_client_state ) {
					case signing_out :	break;
					case signed_out :
						close_connection	( true );
						break;
					case signing_in :
						break;
					case signed_in :
						sign_out			( boost::function< void (	connection_error_types_enum,
																		handshaking_error_types_enum,
																		socket_error_types_enum,
																		login_server_message_types_enum ) >( ) );
				}
				break;
			case handshaked :
				switch ( m_client_state ) {
					case signing_out :	break;
					case signed_out :
						close_connection	( true );
						break;
					case signing_in :
						break;
					case signed_in :
						sign_out			( boost::function< void (	connection_error_types_enum,
																		handshaking_error_types_enum,
																		socket_error_types_enum,
																		login_server_message_types_enum ) >( ) );
				}
		}

		m_io_service.run_one	( );
	} while ( m_connection_state != unresolved );
}

bool login_client_impl::verify_ssl_certificate( const bool preverified, boost::asio::ssl::verify_context& verify_context )
{
	VOSTOK_UNREFERENCED_PARAMETER	( verify_context );

	// the certificate chain has already been checked against
	// the login server certificate loaded in the constructor
	// (load_verify_file), so we just propagate the result of
	// the openssl verification here
	//
	// in case the certificate is not valid, the handshake
	// fails and the login attempt is reported to the caller
	// via the corresponding handshaking error
	//
	// see also boost::asio::ssl::rfc2818_verification
	// and boost::asio::ssl::stream::set_verify_callback

	return preverified;
}

void login_client_impl::establish_connection( boost::function< void ( connection_error_types_enum ) > const& functor, const u32 resolve_retry_count, const u32 reconnect_retry_count )
{
	if ( m_connection_state == unresolved )
		resolve	(
			boost::bind(
				&login_client_impl::connect,
				this,
				_1,
				_2,
				reconnect_retry_count,
				functor
			),
			resolve_retry_count
		);
}

// LOG_INFO must remain at target source line 133; its retained __LINE__ value
// affects callback construction (log-callback-ctor-schedule.md).
void login_client_impl::close_connection( const bool stop_ping_timer )
{
	LOG_INFO				( "[LOGIN] closed connection\r\n" );

	if ( stop_ping_timer )
		m_ping_timer.cancel	( );

	m_ssl_stream.~stream	( );
	new ( &m_ssl_stream ) boost::asio::ssl::stream< boost::asio::ip::tcp::socket& >( m_socket, m_ssl_context );

	m_socket.shutdown		( boost::asio::ip::tcp::socket::shutdown_both );
	m_socket.close			( );
	m_connection_state		= unresolved;
}

} // namespace network
} // namespace vostok
