// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// claude@NOTE: STRUCTURE MATCH (16/16). Residual is the boost::function4::operator()
// inline-vs-call wall: target out-of-lines the COMDAT (5-byte call), base inlines the
// empty/throw/get_vtable body (+0x3d per callback site x3) + LOG ctor scheduling. Global
// boost-header knob, off-limits from a consumer TU. See template-accessor-deinline-global-knob.md.
void login_client_impl::on_sign_out_password_written(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const&	callback,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred
	)
{

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( true );
		callback			( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type );
		LOG_ERROR			( "[LOGIN] write_password_during_sign_OUT: error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( true );
		callback			( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] on sign out password written\r\n" );

	m_client_state			= signed_out;
	close_connection		( true );
	if ( !m_in_destructor )
		callback			( successfully_connected, successfully_handshaked, no_socket_error, servers_connection_info_message_type );
}
// claude@NOTE: STRUCTURE MATCH (13/13). Residual = function4::operator() inline-vs-call wall
// x1 + the async_write callback bind-copy lowering; global boost-header knob (see note above).
void login_client_impl::on_sign_out_handshaked( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback, const handshaking_error_types_enum error )
{
	if ( error == cannot_handshake ) {
		m_client_state		= signed_out;
		close_connection	( true );
		if ( !m_in_destructor )
			callback		( successfully_connected, cannot_handshake, no_socket_error, servers_connection_info_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] on sign out handshaked\r\n" );

	const u32 password_length	= strlen( m_password );
	pbyte buffer			= m_data;
	*buffer++				= ( u8 )password_length;
	memcpy					( buffer, m_password, password_length );
	buffer					+= password_length;

	boost::asio::async_write	(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client_impl::on_sign_out_password_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
// claude@NOTE: STRUCTURE MATCH (16/16). Residual = function4::operator() inline-vs-call wall
// x2 + LOG ctor scheduling; global boost-header knob (see note above).
void login_client_impl::on_sign_out_written( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback, boost::system::error_code const& error_code, u32 bytes_transferred )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( m_client_state == signing_out ) );

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( true );
		if ( !m_in_destructor )
			callback		( successfully_connected, no_handshake, unable_to_write_to_socket, login_server_invalid_message_type );
		LOG_ERROR			( "[LOGIN] SIGN_OUT: error during writing to socket: %s\r\n", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( true );
		if ( !m_in_destructor )
			callback		( successfully_connected, no_handshake, unable_to_write_to_socket, login_server_invalid_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] on sign out written\r\n" );

	handshake				(
		boost::bind(
			&login_client_impl::on_sign_out_handshaked,
			this,
			callback,
			_1
		),
		login_handshake_retry_count,
		true
	);
}
// claude@NOTE: STRUCTURE MATCH (12/12). Residual = function4::operator() inline-vs-call wall
// x1 + the async_write callback bind-copy lowering; global boost-header knob (see note above).
void login_client_impl::sign_out_on_connected( connection_error_types_enum connection_result, boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback )
{
	if ( connection_result ) {
		m_client_state		= signed_out;
		if ( !m_in_destructor )
			callback		( cannot_connect, no_handshake, no_socket_error, login_server_invalid_message_type );
		return;
	}

	LOG_INFO				( "[LOGIN] sign out - on connected\r\n" );

	pbyte buffer			= m_data;

	*buffer++				= ( u8 )sign_out_message_type;

	*( u32* )buffer			= m_session_id;
	buffer					+= sizeof( m_session_id );

	m_client_state			= signing_out;
	boost::asio::async_write	(
		m_socket,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind
		(
			&login_client_impl::on_sign_out_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
// claude@NOTE: STRUCTURE MATCH (11/11). Residual = the function1(bind_t) conversion lowering
// at the establish_connection boundary + LOG ctor scheduling; global boost-header knob (see note above).
void login_client_impl::sign_out( boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& callback )
{
	boost::function< void ( connection_error_types_enum ) > const& sign_out_functor	=
		boost::bind(
			&login_client_impl::sign_out_on_connected,
			this,
			_1,
			callback
		);

	LOG_INFO				( "[LOGIN] signing out...\r\n" );

	if ( m_client_state == signed_out )
		LOG_ERROR			( "[LOGIN] client has already been signed out\r\n" );
	else if ( m_client_state == signed_in ) {
		if ( m_connection_state == unresolved )
			establish_connection	( sign_out_functor, login_resolve_retry_count, login_connect_retry_count );
		else
			sign_out_functor	( successfully_connected );
	}
	else
		LOG_ERROR			( "[LOGIN] waiting for previous operation to be completed\r\n" );
}

} // namespace network
} // namespace vostok
