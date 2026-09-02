// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LOGIN_CLIENT_IMPL_H_INCLUDED
#define LOGIN_CLIENT_IMPL_H_INCLUDED

#include <boost/asio/ssl.hpp>
#include <vostok/login_server/message_types.h>
#include <vostok/login_server/constants.h>

namespace vostok {

struct sign_up_info;

namespace network {

class login_client_impl : private boost::noncopyable {
public:
	enum client_state_enum
	{
		signing_out		= 0x0,
		signed_out		= 0x1,
		signing_in		= 0x2,
		signed_in		= 0x3,
		signing_up		= 0x4,
	}; // enum client_state_enum

	enum connection_state_enum
	{
		unresolved		= 0x0,
		resolving		= 0x1,
		resolved		= 0x2,
		connecting		= 0x3,
		connected		= 0x4,
		handshaking		= 0x5,
		handshaked		= 0x6,
	}; // enum connection_state_enum

	enum
	{
		ping_retry_count	= 0xa,
	};

public:
			explicit	login_client_impl					( boost::asio::io_service& io_service );
						~login_client_impl					( );

			void		sign_up								(
							pcstr					host,
							u16						port,
							sign_up_info const&		sign_up_info,
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback
						);
			void		sign_in								(
							pcstr		host,
							u16			port,
							pcstr		account_name,
							pcstr		password,
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
						);
			void		sign_out							(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
						);

	// claude@NOTE: no standalone symbols; session_id/host_ip/server_browser_*
	// expansions byte-verified via login_client::{session_id,host_ip_address,
	// server_browser_address,server_browser_initial_query} (all 100%); the
	// is_signed_* pair has no matched consumer yet (state values consistent with
	// ping()'s inlined `m_client_state != signed_in` cmp 3)
	inline	bool		is_signed_in						( ) const { return m_client_state == signed_in; }
	inline	bool		is_signed_out						( ) const { return m_client_state == signed_out; }

	inline	u32			session_id							( ) const { return m_session_id; }

	inline	pcstr		host_ip								( ) const { return m_host_ip; }

	inline	pcstr		server_browser_address				( ) const { return m_server_browser_address; }
	inline	pcstr		server_browser_initial_query		( ) const { return m_server_browser_initial_query; }

private:
			bool		verify_ssl_certificate				( const bool preverified, boost::asio::ssl::verify_context& verify_context );

			void		on_resolved							(
							boost::asio::ip::tcp::resolver* const	resolver,
							const u32							retry_count,
							boost::function< void ( enum resolve_error_types_enum, boost::asio::ip::tcp::resolver::iterator ) > const&	functor,
							boost::system::error_code const&	error_code,
							boost::asio::ip::tcp::resolver::iterator	iterator
						);

			void		resolve								(
							boost::function< void ( enum resolve_error_types_enum, boost::asio::ip::tcp::resolver::iterator ) > const&	functor,
							const u32		retry_count
						);

			void		on_connected						(
							const u32							retry_count,
							boost::function< void ( enum connection_error_types_enum ) > const&	functor,
							boost::asio::ip::tcp::resolver::iterator	start_iterator,
							boost::system::error_code const&	error_code,
							boost::asio::ip::tcp::resolver::iterator	iterator
						);

			void		connect								(
							resolve_error_types_enum	error,
							boost::asio::ip::tcp::resolver::iterator	iterator,
							const u32					retry_count,
							boost::function< void ( enum connection_error_types_enum ) > const&	functor
						);

	// claude@NOTE: no target symbol and no call site in any matched target body
	// (the connect/handshake chain never binds it) - dead inline source
	inline	void		on_handshake_connected				(
							const connection_error_types_enum		connection_result,
							boost::function< void ( enum handshaking_error_types_enum ) > const&	functor,
							const u32								retry_count,
							bool									stop_timer
						)
	{
		VOSTOK_UNREFERENCED_PARAMETERS	( &connection_result, &functor, &retry_count, &stop_timer );
	}

			void		on_handshaked						(
							boost::system::error_code const&	error_code,
							boost::function< void ( enum handshaking_error_types_enum ) > const&	functor,
							u32									retry_count,
							bool								stop_timer
						);

			void		handshake							(
							boost::function< void ( enum handshaking_error_types_enum ) > const&	functor,
							u32		retry_count,
							bool	stop_timer
						);

			void		establish_connection				(
							boost::function< void ( enum connection_error_types_enum ) > const&	functor,
							const u32		resolve_retry_count,
							const u32		reconnect_retry_count
						);
			void		close_connection					( bool stop_ping_timer );

			void		on_sign_in_answer_received			(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);
			void		on_sign_in_password_written			(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);
			void		on_sign_in_handshaked				(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							const handshaking_error_types_enum	error
						);
			void		on_user_name_answer_received		(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);
			void		on_sign_in_written					(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							boost::system::error_code const&	error_code,
							u32									bytes_transferred
						);

			void		sign_in_on_connected				(
							connection_error_types_enum		connection_result,
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
						);

			void		on_sign_out_password_written		(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);
			void		on_sign_out_handshaked				(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							const handshaking_error_types_enum	error
						);
			void		on_sign_out_written					(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
							boost::system::error_code const&	error_code,
							u32									bytes_transferred
						);

			void		sign_out_on_connected				(
							connection_error_types_enum		connection_result,
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
						);

			void		on_sign_up_answer_received			(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
							sign_up_info const&					sign_up_info,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);
			void		on_sign_up_info_written				(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
							sign_up_info const&					sign_up_info,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);

			void		sign_up_on_handshaked				(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
							sign_up_info const&					sign_up_info,
							const handshaking_error_types_enum	handshaking_result
						);

			void		on_sign_up_account_answer_received	(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
							sign_up_info const&					sign_up_info,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);
			void		on_sign_up_written					(
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
							sign_up_info const&					sign_up_info,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);

			void		sign_up_on_connected				(
							const connection_error_types_enum	connection_result,
							boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
							sign_up_info const&					sign_up_info
						);

			void		on_ping_sent						(
							const u32							try_count,
							boost::system::error_code const&	error_code,
							const u32							bytes_transferred
						);

			void		ping								( u32 retry_count );

private:
	boost::asio::ip::tcp::socket	m_socket;
	boost::asio::ssl::context		m_ssl_context;
	boost::asio::ssl::stream< boost::asio::ip::tcp::socket& >	m_ssl_stream;
	boost::asio::ip::udp::socket	m_ping_socket;
	boost::asio::deadline_timer		m_ping_timer;
	boost::asio::io_service&		m_io_service;
	client_state_enum				m_client_state;
	connection_state_enum			m_connection_state;
	u32								m_session_id;
	u16								m_host_port;
	bool							m_in_destructor;
	char							m_host[64];
	char							m_password[48];
	char							m_account_name[32];
	char							m_host_ip[16];
	u8								m_data[135];
	char							m_server_browser_address[256];
	char							m_server_browser_initial_query[256];
}; // class login_client_impl

STATIC_SIZE_ASSERT(login_client_impl, 0x4A0);

} // namespace network
} // namespace vostok

#endif // #ifndef LOGIN_CLIENT_IMPL_H_INCLUDED
