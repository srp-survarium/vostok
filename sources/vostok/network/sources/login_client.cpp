// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/network/login_client.h>
#include "network_world.h"
#include "login_client_impl.h"
#include "string_order.h"
#include "functor_order.h"
#include "functor_response.h"
#include <vostok/console_command.h>
#include <vostok/network_core/sources/network_core_entry_point.h>

// Target linkage keeps these buffers at global scope.
char	s_net_client_account_name[128];
char	s_net_client_account_password_[128];

// The target initializes both commands with a 128-byte user-specific buffer.
static vostok::console_commands::cc_string s_net_client_account_name_cc(
	"account_name", s_net_client_account_name,
	sizeof( s_net_client_account_name ), true,
#line 23
	vostok::console_commands::command_type_user_specific );
static vostok::console_commands::cc_string s_net_client_account_password_cc(
	"account_password", s_net_client_account_password_,
	sizeof( s_net_client_account_password_ ), true,
#line 24
	vostok::console_commands::command_type_user_specific );

namespace vostok {
namespace network {

pcstr login_client::account_name( ) const
#line 29
{
	return				s_net_client_account_name;
}

pcstr login_client::account_password( ) const
{
	return				m_net_client_account_password;
}



void login_client::store_user_password_in_settings( )
#line 39
{
	strings::copy		( s_net_client_account_password_, m_net_client_account_password );
}

void login_client::reset_user_password_in_settings( )
{
	s_net_client_account_password_[0]	= 0;
}



void login_client::create_client( )
#line 49
{
	ASSERT				( UNKNOWN_EXPRESSION_T( !m_client ) );
	m_client			= NEW( login_client_impl ) ( m_world.io_service( ) );
	std::string ip_address	= network_core::get_ip_address( m_world.io_service( ) );
	strings::copy		( m_local_host_ip, ip_address.c_str( ) );
}





login_client::login_client( world& world ) :
	m_world				( static_cast_checked<network_world&>(world) ),
	m_client			( 0 ),
	m_client_state		( signed_out )
#line 60
{
	strings::copy		( m_net_client_account_password, s_net_client_account_password_ );

	m_local_host_ip[0]	= 0;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &login_client::create_client, this )
		)
#line 68
	);
}

} // namespace network
} // namespace vostok





// Global scope: retail names this static the unmangled `destroy_client`.
static void destroy_client( vostok::network::login_client_impl* client_to_destroy )
#line 72
{
	VOSTOK_DELETE_IMPL	( vostok::network::g_allocator, client_to_destroy );
}

namespace vostok {
namespace network {



login_client::~login_client( )
#line 77
{
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &destroy_client, m_client )
		)
	);
}






void login_client::on_signed_up(
		connection_error_types_enum			connection_error,
		handshaking_error_types_enum		handshaking_error,
		socket_error_types_enum				socket_error,
		login_server_message_types_enum		login_error,
		sign_up_info const&					sign_up_info
	)
#line 92
{
	m_client_state		= signed_out;

	if ( !m_on_sign_up )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( m_on_sign_up, connection_error, handshaking_error, socket_error, login_error, sign_up_info )
		)
	);
}




void login_client::on_signed_in(
		connection_error_types_enum			connection_error,
		handshaking_error_types_enum		handshaking_error,
		socket_error_types_enum				socket_error,
		login_server_message_types_enum		login_error
	)
#line 132
{
#line 138
	if ( !connection_error && !handshaking_error && !socket_error && ( login_error == servers_connection_info_message_type ) )
		m_client_state	= signed_in;
	else
		m_client_state	= signed_out;

	if ( !m_on_sign_in )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( m_on_sign_in, connection_error, handshaking_error, socket_error, login_error )
		)
	);
}



void login_client::sign_in_impl(
		pcstr const		host,
		const u16		port,
		pcstr const		account_name,
		pcstr const		password
	)
#line 154
{
	m_client->sign_in	( host, port, account_name, password, boost::bind( &login_client::on_signed_in, this, _1, _2, _3, _4 ) );
}

// Target emits this only inline; m_sign_up_info must outlive the asynchronous
// boost::ref chain.
void login_client::sign_up(
		pcstr					host,
		u16						port,
		sign_up_info const&		info,
		boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, sign_up_info const& ) > const&	callback
	)
{
	m_sign_up_info		= info;
	m_on_sign_up		= callback;
	m_client->sign_up	( host, port, m_sign_up_info, boost::bind( &login_client::on_signed_up, this, _1, _2, _3, _4, _5 ) );
}





void login_client::sign_in(
		pcstr			host,
		u16				port,
		pcstr			account_name,
		pcstr			password,
		boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
	)
#line 159
{
	strings::copy		( m_server_host, host );
	m_server_port		= port;

	strings::copy		( s_net_client_account_name, account_name );
	strings::copy		( m_net_client_account_password, password );

	m_on_sign_in		= callback;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), string_order ) (
			m_world.orders_allocator( ),
			boost::bind(
				&login_client::sign_in_impl,
				this,
				_1,
				port,
				_2,
				_3
			),
			m_server_host,
			s_net_client_account_name,
			m_net_client_account_password
		)
#line 182
	);
}



void login_client::on_signed_out(
		connection_error_types_enum			connection_error,
		handshaking_error_types_enum		handshaking_error,
		socket_error_types_enum				socket_error,
		login_server_message_types_enum		login_error
	)
#line 191
{
	m_client_state		= signed_out;

	if ( !m_on_sign_out )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( m_on_sign_out, connection_error, handshaking_error, socket_error, login_error )
		)
	);
}





void login_client::sign_out(
		boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
	)
#line 205
{
	m_on_sign_out		= callback;
	boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& on_signed_out	= boost::bind( &login_client::on_signed_out, this, _1, _2, _3, _4 );

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind(
				&login_client_impl::sign_out,
				m_client,
				on_signed_out
			)
		)
#line 216
	);
}

u32 login_client::session_id( ) const
{
	return				m_client->session_id( );
}

pcstr login_client::host_ip_address( ) const
{
	return				m_client->host_ip( );
}

pcstr login_client::server_browser_address( ) const
{
	return				m_client->server_browser_address( );
}

pcstr login_client::server_browser_initial_query( ) const
{
	return				m_client->server_browser_initial_query( );
}

} // namespace network
} // namespace vostok
