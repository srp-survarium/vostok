// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED

#include <vostok/network_core/tcp_packet_socket.h>
#include <vostok/network_core/async_connector.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class tcp_packet;

class tcp_packet_client : private boost::noncopyable {
public:
			explicit					tcp_packet_client			( boost::asio::io_service& io_service );
										~tcp_packet_client			( );

			void						connect						( pcstr host, u16 port );
			void						disconnect					( );

			void						send						( tcp_packet const& packet );

	inline	bool						is_connected				( ) const { return false; }
	inline	bool						has_connection_established	( ) const { return false; }

	// STATE[INLINED]: bodies from network::tcp_packet_client::create_client, which
	// calls m_client->set_on_*( ... ) and inline-folds the `m_x = value;` store; the
	// sibling network::tcp_packet_client::set_on_connected is the matched out-of-line
	// form of the same body (100%).
	inline	void						set_on_connected			( boost::function< void() > const& value ) { m_on_connected = value; /* no source */ }
	inline	void						set_on_disconnected			( boost::function< void() > const& value ) { m_on_disconnected = value; /* no source */ }
	inline	void						set_on_packet_received		( boost::function< void( tcp_packet const& ) > const& value ) { m_on_packet_received = value; /* no source */ }
	inline	void						set_on_error				(
											boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > const&	value
										) { m_on_error = value; /* no source */ }

	inline	boost::asio::io_service&	io_service					( ) { return m_io_service; }

private:
			void						on_connected				( );
			void						close_connection			( );
			void						on_error					( client_error_codes_enum client_error_code, boost::system::error_code error_code );
			void						start_reading				( );

private:
	/* 0x0000 */	boost::asio::ip::tcp::socket								m_socket;
	/* 0x0048 */	tcp_packet_socket< boost::asio::ip::tcp::socket >			m_packet_socket;
	/* 0x04a0 */	async_connector											m_async_connector;
	/* 0x0900 */	boost::function< void() >									m_on_connected;
	/* 0x0920 */	boost::function< void() >									m_on_disconnected;
	/* 0x0940 */	boost::function< void( tcp_packet const& ) >				m_on_packet_received;
	/* 0x0960 */	boost::function< void( enum client_error_codes_enum, boost::system::error_code ) >	m_on_error;
	/* 0x0980 */	boost::asio::io_service&									m_io_service;
	/* 0x0984 */	tcp_packet*												m_first_packet;
}; // class tcp_packet_client

STATIC_SIZE_ASSERT(tcp_packet_client, 0x988);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED
