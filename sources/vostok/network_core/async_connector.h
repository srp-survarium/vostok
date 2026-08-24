////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

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
	enum connection_state_enum
	{
		host_name_is_unresolved				= 0x0,
		host_name_is_being_resolved			= 0x1,
		host_name_has_been_resolved			= 0x2,
		connection_is_being_established		= 0x3,
		connection_has_been_established		= 0x4,
	};

					async_connector				( );

			void	connect						(
						boost::asio::ip::tcp::socket&		socket,
						pcstr								host,
						u16									host_port,
						boost::function< void() > const&	on_connected,
						boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > const&	on_error
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

	// STATE[REMOVED] (resolve/close_connection/on_error): the matched async_connector.cpp
	// inlines the resolve/connect/error logic directly into connect()/on_resolved()/
	// on_connected() and never calls these private helpers; no other TU can (they're
	// private, no friends). Uninstantiated both sides.
	inline	void	resolve						( pcstr host, u32 port ) { /* no source */ } // STATE[REMOVED]

	inline	void	close_connection			( ) { /* no source */ } // STATE[REMOVED]

	inline	void	on_error					( client_error_codes_enum client_error_code, boost::system::error_code error_code ) { /* no source */ } // STATE[REMOVED]

public:
	// trivial RAII dtor (resolver-iterator/function/socket-ptr/handler_allocator members
	// auto-destroy); the empty body is correct - this is the matched ??1async_connector form.
	inline			~async_connector			( ) { /* no source */ }

private:
	/* 0x0000 */	boost::asio::ip::tcp::resolver::iterator	m_host;
	/* 0x0010 */	boost::function< void() >			m_on_connected;
	/* 0x0030 */	boost::function< void( enum client_error_codes_enum, boost::system::error_code ) >	m_on_error;
	/* 0x0050 */	handler_allocator					m_allocator;
	/* 0x0458 */	boost::asio::ip::tcp::socket*		m_socket;
	/* 0x045c */	connection_state_enum				m_connection_state;
}; // class async_connector

STATIC_SIZE_ASSERT(async_connector, 0x460);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED
