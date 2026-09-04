// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED
#define NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED

#include <vostok/network_core/handler_allocator.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <boost/function.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class async_connector {
public:
	typedef boost::asio::ip::tcp::socket									socket_type;
	typedef boost::function< void() >										on_connected_type;
	typedef boost::function< void( enum client_error_codes_enum, boost::system::error_code ) >	on_error_type;

					async_connector				( );

			void	connect						(
						socket_type&						socket,
						pcstr								host,
						u16									host_port,
						on_connected_type const&			on_connected,
						on_error_type const&				on_error
					);
private:
			void	connect						( boost::asio::ip::tcp::resolver::iterator const& iterator );

public:
			void	reset						( );

	inline	bool	is_connected				( ) const { return m_connection_state >= connection_is_being_established; }

	inline	bool	has_connection_established	( ) const { return m_connection_state == connection_has_been_established; }

private:
			void	on_connected				(
						boost::system::error_code const&	error_code,
						boost::asio::ip::tcp::resolver::iterator	iterator
					);
			void	on_resolved					(
						boost::asio::ip::tcp::resolver* const	resolver,
						boost::system::error_code const&	error_code,
						boost::asio::ip::tcp::resolver::iterator	iterator
					);

	// STATE[UNMATCHABLE]: the matched async_connector.cpp
	// inlines the resolve/connect/error logic directly into connect()/on_resolved()/
	// on_connected() and never calls these private helpers; no other TU can (they're
	// private, no friends), so the shipped image exposes no bodies.
	inline	void	resolve						( pcstr host, u32 port ) { /* no source */ }

	inline	void	close_connection			( ) { /* no source */ }

	inline	void	on_error					( client_error_codes_enum client_error_code, boost::system::error_code error_code ) { /* no source */ }

private:
	enum connection_state_enum
	{
		host_name_is_unresolved				= 0x0,
		host_name_is_being_resolved			= 0x1,
		host_name_has_been_resolved			= 0x2,
		connection_is_being_established		= 0x3,
		connection_has_been_established		= 0x4,
	};

	/* 0x0000 */	boost::asio::ip::tcp::resolver::iterator	m_host;
	/* 0x0010 */	on_connected_type					m_on_connected;
	/* 0x0030 */	on_error_type						m_on_error;
	/* 0x0050 */	handler_allocator					m_allocator;
	/* 0x0458 */	socket_type*						m_socket;
	/* 0x045c */	connection_state_enum				m_connection_state;
}; // class async_connector

STATIC_SIZE_ASSERT(async_connector, 0x460);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED
