// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_NETWORK_LOGIN_CLIENT_H_INCLUDED
#define VOSTOK_NETWORK_LOGIN_CLIENT_H_INCLUDED

#include <vostok/network/api.h>
#include <vostok/login_server/login_structures.h>
#include <vostok/login_server/message_types.h>

namespace survarium { class game; }

namespace vostok {

namespace network {

struct world;
class network_world;
class login_client_impl;

class VOSTOK_NETWORK_API login_client : private core::noncopyable {
public:
	enum client_state_enum
	{
		signed_out		= 0x0,
		signed_in		= 0x1,
	}; // enum client_state_enum

public:
			explicit			login_client					( world& world );
								~login_client					( );

			void				sign_up							(
									pcstr					host,
									u16						port,
									sign_up_info const&		info,
									boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, sign_up_info const& ) > const&	callback
								);

			void				sign_in							(
									pcstr		host,
									u16			port,
									pcstr		account_name,
									pcstr		password,
									boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
								);
			void				sign_out						(
									boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
								);

	// claude@NOTE: no standalone symbols and no matched consumer; bodies are the
	// canonical state-compare one-liners (values pinned by the matched ctor/
	// on_signed_* stores)
	inline	bool				is_signed_in					( ) const { return m_client_state == signed_in; }
	inline	bool				is_signed_out					( ) const { return m_client_state == signed_out; }
	inline	client_state_enum	client_state					( ) const { return m_client_state; }

			pcstr				account_name					( ) const;
			pcstr				account_password				( ) const;

			u32					session_id						( ) const;

			pcstr				server_browser_address			( ) const;
			pcstr				server_browser_initial_query	( ) const;

	// claude@NOTE: no standalone symbol and no matched consumer; the member it
	// returns is pinned by create_client's strings::copy( m_local_host_ip, ... )
	inline	pcstr				local_ip_address				( ) const { return m_local_host_ip; }
			pcstr				host_ip_address					( ) const;

			void				store_user_password_in_settings	( );
			void				reset_user_password_in_settings	( );

private:
			void				create_client					( );

			void				sign_in_impl					(
									pcstr const		host,
									const u16		port,
									pcstr const		account_name,
									pcstr const		password
								);

			void				on_signed_up					(
									connection_error_types_enum			connection_error,
									handshaking_error_types_enum		handshaking_error,
									socket_error_types_enum				socket_error,
									login_server_message_types_enum		login_error,
									sign_up_info const&					sign_up_info
								);
			void				on_signed_in					(
									connection_error_types_enum			connection_error,
									handshaking_error_types_enum		handshaking_error,
									socket_error_types_enum				socket_error,
									login_server_message_types_enum		login_error
								);
			void				on_signed_out					(
									connection_error_types_enum			connection_error,
									handshaking_error_types_enum		handshaking_error,
									socket_error_types_enum				socket_error,
									login_server_message_types_enum		login_error
								);

private:
	sign_up_info				m_sign_up_info;
	boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, sign_up_info const& ) >	m_on_sign_up;
	boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) >	m_on_sign_in;
	boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) >	m_on_sign_out;
	network_world&				m_world;
	login_client_impl*			m_client;
	client_state_enum			m_client_state;

public:
	char						m_local_host_ip[16];
	char						m_server_host[64];
	u16							m_server_port;
	char						m_net_client_account_password[128];
}; // class login_client

STATIC_SIZE_ASSERT(login_client, 0x1F8);

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_LOGIN_CLIENT_H_INCLUDED
