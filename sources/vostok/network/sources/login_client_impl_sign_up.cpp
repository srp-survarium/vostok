////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"
#include <vostok/login_server/login_structures.h>

namespace vostok {
namespace network {

// STATE[89.93%|PARTIAL]: structure clean (function5::operator() INLINED in BOTH - target matches the base inline here); residual = the callback/info bind-copy lowering (function5 copy + boost::ref slot) vs the target folded-ctor schedule + LOG-helper scheduling
void login_client_impl::on_sign_up_answer_received(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
		sign_up_info const&					sign_up_info,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred
	)
{

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_server_invalid_message_type, sign_up_info );
		LOG_ERROR			( "[LOGIN] on_SIGN_UP_answer_received: error during reading from socket: %s", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_server_invalid_message_type, sign_up_info );
		return;
	}

	m_client_state			= signed_out;
	close_connection		( false );
	if ( !m_in_destructor )
		callback			( successfully_connected, successfully_handshaked, no_socket_error, ( login_server_message_types_enum )m_data[0], sign_up_info );

	// STRUCTURE DIFF: target 17 stmts / base 17 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - callback/info bind-copy lowering + LOG-helper scheduling; non-steerable LTCG.
}

// STATE[89.71%|PARTIAL]: structure clean; residual = the callback/info bind-copy lowering (function5 copy + boost::ref slot) vs the target folded-ctor schedule + LOG-helper scheduling
void login_client_impl::on_sign_up_info_written(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
		sign_up_info const&					sign_up_info,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( m_client_state == signing_up ) );

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type, sign_up_info );
		LOG_ERROR			( "[LOGIN] on_SIGN_UP_password_written: error during writing to socket: %s", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type, sign_up_info );
		return;
	}

	m_ssl_stream.async_read_some	(
		boost::asio::buffer( m_data, 1 ),
		boost::bind(
			&login_client_impl::on_sign_up_answer_received,
			this,
			callback,
			boost::ref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - callback/info bind-copy lowering + LOG-helper scheduling; non-steerable LTCG.
}

// STATE[91.78%|PARTIAL]: structure clean (password/email length-prefixed writes + both ASSERT eaters verified); residual = the callback/info bind-copy lowering (function5 copy + boost::ref slot) vs the target folded-ctor schedule
void login_client_impl::sign_up_on_handshaked(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
		sign_up_info const&					sign_up_info,
		const handshaking_error_types_enum	handshaking_result )
{
	if ( handshaking_result ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, handshaking_result, invalid_socket_error_type, login_server_invalid_message_type, sign_up_info );
		return;
	}

	pbyte buffer			= m_data;

	const u8 password_length	= ( u8 )strlen( sign_up_info.password );
	ASSERT					( UNKNOWN_EXPRESSION_T( password_length < sizeof( sign_up_info.password ) ) );
	*buffer++				= password_length;
	memcpy					( buffer, sign_up_info.password, password_length );
	buffer					+= password_length;

	const u8 email_length	= ( u8 )strlen( sign_up_info.email );
	ASSERT					( UNKNOWN_EXPRESSION_T( email_length < sizeof( sign_up_info.email ) ) );
	*buffer++				= email_length;
	memcpy					( buffer, sign_up_info.email, email_length );
	buffer					+= email_length;

	boost::asio::async_write	(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client_impl::on_sign_up_info_written,
			this,
			callback,
			boost::ref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

// STATE[88.75%|PARTIAL]: structure 26/26 (default-case `return` restored - the
// target's 2-byte jmp goes to the epilogue, skipping handshake, same shape as
// on_user_name_answer_received); residual = the callback/info bind-copy lowering
// (function5 copy + boost::ref slot) vs the target folded-ctor schedule +
// LOG-helper scheduling
void login_client_impl::on_sign_up_account_answer_received(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
		sign_up_info const&					sign_up_info,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred )
{
	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_server_invalid_message_type, sign_up_info );
		LOG_ERROR			( "[LOGIN] on_SIGN_UP_answer_received: error during reading from socket: %s", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_server_invalid_message_type, sign_up_info );
		return;
	}

	switch ( m_data[0] ) {
		case occupied_user_name_message_type :
			m_client_state	= signed_out;
			close_connection( false );
			if ( !m_in_destructor )
				callback	( successfully_connected, successfully_handshaked, no_socket_error, ( login_server_message_types_enum )m_data[0], sign_up_info );
			return;

		case send_sign_up_info_message_type :
			break;

		default :
			m_client_state	= signed_out;
			close_connection( false );
			if ( !m_in_destructor )
				callback	( successfully_connected, successfully_handshaked, no_socket_error, ( login_server_message_types_enum )m_data[0], sign_up_info );
			return;
	}

	handshake				(
		boost::bind
		(
			&login_client_impl::sign_up_on_handshaked,
			this,
			callback,
			boost::ref( sign_up_info ),
			_1
		),
		login_handshake_retry_count,
		false
	);

	// STRUCTURE DIFF: target 26 stmts / base 26 stmts
	// SIZE -0x4 | 137 | LOG_ERROR ( "[LOGIN] on_SIGN_UP_answer_received: ...", ... );
	// SIZE +0x3 | 165 | return;
	// SIZE +0x20| 179 | );
	// VERDICT: STRUCTURE MATCH - quantity fixed (default-case return); residuals are the function5 bind-copy lowering + jmp-near/short distance bloat; non-steerable LTCG.
}

// STATE[89.66%|PARTIAL]: structure clean; residual = the callback/info bind-copy lowering (function5 copy + boost::ref slot) vs the target folded-ctor schedule + LOG-helper scheduling
void login_client_impl::on_sign_up_written(
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
		sign_up_info const&					sign_up_info,
		boost::system::error_code const&	error_code,
		const u32							bytes_transferred )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( m_client_state == signing_up ) );

	if ( error_code ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type, sign_up_info );
		LOG_ERROR			( "[LOGIN] SIGN_UP: error during writing to socket: %s", error_code.message( ).c_str( ) );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state		= signed_out;
		close_connection	( false );
		if ( !m_in_destructor )
			callback		( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_server_invalid_message_type, sign_up_info );
		return;
	}

	m_socket.async_read_some	(
		boost::asio::buffer( m_data, 1 ),
		boost::bind(
			&login_client_impl::on_sign_up_account_answer_received,
			this,
			callback,
			boost::ref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - callback/info bind-copy lowering + LOG-helper scheduling; non-steerable LTCG.
}

// STATE[90.66%|PARTIAL]: structure clean (sign_up_message_type byte + account_name length-prefixed write verified); residual = the callback/info bind-copy lowering (function5 copy + boost::ref slot) vs the target folded-ctor schedule
void login_client_impl::sign_up_on_connected(
		const connection_error_types_enum	connection_result,
		boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
		sign_up_info const&					sign_up_info )
{
	if ( connection_result ) {
		m_client_state		= signed_out;
		if ( !m_in_destructor )
			callback		( cannot_connect, handshaking_error_type, invalid_socket_error_type, login_server_invalid_message_type, sign_up_info );
		return;
	}

	m_client_state			= signing_up;

	pbyte buffer			= m_data;

	*buffer++				= ( u8 )sign_up_message_type;

	const u8 account_name_length	= ( u8 )strlen( sign_up_info.account_name );
	ASSERT					( UNKNOWN_EXPRESSION_T( account_name_length < sizeof( sign_up_info.account_name ) ) );
	*buffer++				= account_name_length;
	memcpy					( buffer, sign_up_info.account_name, account_name_length );
	buffer					+= account_name_length;

	boost::asio::async_write	(
		m_socket,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client_impl::on_sign_up_written,
			this,
			callback,
			boost::ref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}
// STATE[84.39%|PARTIAL]: structure 4/4; residual = the function1(bind_t) conversion lowering at the establish_connection boundary (bind carries the function5 copy + ref slot)
void login_client_impl::sign_up( pcstr host, u16 port, sign_up_info const& sign_up_info, boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, vostok::sign_up_info const& ) > const& callback )
{
	m_host_port				= port;
	strncpy_s				( m_host, sizeof( m_host ), host, sizeof( m_host ) - 1 );
	m_host[ sizeof( m_host ) - 1 ]	= 0;

	establish_connection	(
		boost::bind(
			&login_client_impl::sign_up_on_connected,
			this,
			_1,
			callback,
			boost::ref( sign_up_info )
		),
		login_resolve_retry_count,
		login_connect_retry_count
	);

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts (SIZE-only, rva-pinned 0x7947f0)
	// VERDICT: STRUCTURE MATCH - function1(bind_t) conversion lowering at the establish_connection boundary; non-steerable LTCG.
}

} // namespace network
} // namespace vostok
