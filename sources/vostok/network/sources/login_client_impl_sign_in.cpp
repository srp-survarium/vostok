// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// The target keeps the answer dispatch scoped and records both length locals as non-const.
void login_client_impl::on_sign_in_answer_received(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const&	callback,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred
	)
{


	ASSERT					( UNKNOWN_EXPRESSION_T( m_client_state == signing_in ) );

	if ( error_code || !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( error_code )
			LOG_ERROR		( "[LOGIN] error during reading sign in answer: %s\r\n", error_code.message( ).c_str( ) );

		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, no_socket_error, sign_in_attempt_interval_violated_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] answer has been received!\r\n" );
	pbyte buffer			= m_data;
	switch ( *buffer ) {
		case servers_connection_info_message_type : {
			++buffer;

			u8 length1			= *buffer++;
			m_server_browser_address[0]			= 0;
			m_server_browser_initial_query[0]	= 0;


			memcpy				( m_server_browser_address, buffer, length1 );
			buffer				+= length1;
			m_server_browser_address[length1]	= 0;

			u8 length2			= *buffer++;
			memcpy				( m_server_browser_initial_query, buffer, length2 );
			buffer				+= length2;
			m_server_browser_initial_query[length2]	= 0;

			m_session_id		= *( u32* )buffer;
			buffer				+= sizeof( m_session_id );

			char port[6];
			_itoa_s				( login_udp_port, port, sizeof( port ), 10 );

			boost::asio::ip::udp::resolver resolver( m_io_service );
			boost::asio::ip::udp::resolver::query query(
				m_socket.remote_endpoint( ).address( ).is_v4( ) ? boost::asio::ip::udp::v4( ) : boost::asio::ip::udp::v6( ),
				m_socket.remote_endpoint( ).address( ).to_string( ),
				port
			);
			boost::asio::ip::udp::resolver::iterator iterator	= resolver.resolve( query );
			m_ping_socket.connect	( iterator->endpoint( ) );

			m_client_state		= signed_in;
			close_connection	( false );
			if ( !m_in_destructor )
				callback		( successfully_connected, successfully_handshaked, no_socket_error, servers_connection_info_message_type );

			m_ping_timer.async_wait	( boost::bind( &login_client_impl::ping, this, ping_retry_count ) );
			break;
		}
		default :
			m_client_state		= signed_out;
			if ( !m_in_destructor )
				callback		( successfully_connected, successfully_handshaked, no_socket_error, ( login_server_message_types_enum )*buffer );
	}
}

void login_client_impl::on_sign_in_password_written(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const&	callback,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred
	)
{

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type );
		LOG_ERROR			( "[LOGIN] write_password_during_SIGN_IN: error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] password has been written!\r\n" );
	LOG_INFO				( "[LOGIN] waiting for answer...\r\n" );
	const u32 buffer_size	= 70;
	m_ssl_stream.async_read_some	(
		boost::asio::buffer(
			m_data,
			buffer_size
		),
		boost::bind(
			&login_client_impl::on_sign_in_answer_received,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
void login_client_impl::on_sign_in_handshaked( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback, const handshaking_error_types_enum error )
{
	if ( error == cannot_handshake ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, cannot_handshake, no_socket_error, sign_in_attempt_interval_violated_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] writing password...\r\n" );
	const u32 password_length	= strlen( m_password );
	pbyte buffer			= m_data;
	*buffer++				= ( u8 )password_length;
	memcpy					( buffer, m_password, password_length );
	buffer					+= password_length;

	boost::asio::async_write	(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client_impl::on_sign_in_password_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
// Unknown messages return instead of falling through to the handshake.
void login_client_impl::on_user_name_answer_received( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback, boost::system::error_code const& error_code, const u32 bytes_transferred )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( m_client_state == signing_in ) );

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, no_handshake, unable_to_read_from_socket, login_server_invalid_message_type );
		LOG_ERROR			( "[LOGIN] SIGN_IN: error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, no_handshake, unable_to_read_from_socket, login_server_invalid_message_type );
		return;
	}

	switch ( m_data[0] ) {
		case invalid_user_name_or_password_message_type :
			m_client_state	= signed_out;
			close_connection( false );
			if ( !m_in_destructor )
				callback	( successfully_connected, no_handshake, no_socket_error, ( login_server_message_types_enum )m_data[0] );
			return;

		case valid_user_name_message_type :	break;
		default :
			m_client_state	= signed_out;
			close_connection( false );
			if ( !m_in_destructor )
				callback	( successfully_connected, no_handshake, no_socket_error, ( login_server_message_types_enum )m_data[0] );
			return;
	}

	handshake				(
		boost::bind
		(
			&login_client_impl::on_sign_in_handshaked,
			this,
			callback,
			_1
		),
		login_handshake_retry_count,
		false
	);
}
void login_client_impl::on_sign_in_written( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback, boost::system::error_code const& error_code, u32 bytes_transferred )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( m_client_state == signing_in ) );

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, no_handshake, unable_to_write_to_socket, login_server_invalid_message_type );
		LOG_ERROR			( "[LOGIN] SIGN_IN: error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, no_handshake, unable_to_write_to_socket, login_server_invalid_message_type );
		return;
	}

	boost::asio::async_read	(
		m_socket,
		boost::asio::buffer( m_data, 1 ),
		boost::bind(
			&login_client_impl::on_user_name_answer_received,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
void login_client_impl::sign_in_on_connected( connection_error_types_enum connection_result, boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback )
{
	if ( connection_result ) {
		m_client_state		= signed_out;
		if ( !m_in_destructor )
			callback		( cannot_connect, no_handshake, no_socket_error, login_server_invalid_message_type );
		return;
	}

	pbyte buffer			= m_data;

	*buffer++				= ( u8 )sign_in_message_type;

	const u8 account_name_length	= ( u8 )strlen( m_account_name );
	ASSERT					( UNKNOWN_EXPRESSION_T( account_name_length < sizeof( m_account_name ) ) );
	*buffer++				= account_name_length;
	memcpy					( buffer, m_account_name, account_name_length );
	buffer					+= account_name_length;


	char version[8]			= { 0 };
	strings::copy			( version, "0.100b" );
	memcpy					( buffer, version, sizeof( version ) );
	buffer					+= sizeof( version );

	m_client_state			= signing_in;
	boost::asio::async_write	(
		m_socket,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind
		(
			&login_client_impl::on_sign_in_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
void login_client_impl::sign_in( pcstr host, u16 port, pcstr account_name, pcstr password, boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback )
{
	LOG_INFO				( "signing in to LOGIN Server: host[%s], port[%d], account[%s], password[%s]", host, port, account_name, password );

	m_host_port				= port;
	strncpy_s				( m_host, sizeof( m_host ), host, sizeof( m_host ) - 1 );
	m_host[ sizeof( m_host ) - 1 ]	= 0;

	strncpy_s				( m_account_name, sizeof( m_account_name ), account_name, sizeof( m_account_name ) );
	m_account_name[ sizeof( m_account_name ) - 1 ]	= 0;

	strncpy_s				( m_password, sizeof( m_password ), password, sizeof( m_password ) );
	m_password[ sizeof( m_password ) - 1 ]	= 0;

	boost::function< void ( connection_error_types_enum ) > const& sign_in_functor	=
		boost::bind(
			&login_client_impl::sign_in_on_connected,
			this,
			_1,
			callback
		);

	LOG_INFO				( "[LOGIN] signing in...\r\n" );

	if ( m_connection_state == unresolved )
		establish_connection	( sign_in_functor, login_resolve_retry_count, login_connect_retry_count );
	else if ( m_client_state == signed_out )
		sign_in_functor		( successfully_connected );
	else
		LOG_ERROR			( "[LOGIN] waiting for previous operation to be completed\r\n" );
}

} // namespace network
} // namespace vostok
