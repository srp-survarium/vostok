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

	// STATE[UNMATCHABLE]: retained client API from the legacy implementation; no
	// shipped caller expands these two forwards.
	inline	bool						is_connected				( ) const { return m_async_connector.is_connected( ); }
	inline	bool						has_connection_established	( ) const { return m_async_connector.has_connection_established( ); }

	typedef boost::function< void() >										on_connected_type;
	typedef boost::function< void() >										on_disconnected_type;
	typedef boost::function< void( tcp_packet const& ) >						on_packet_received_type;
	typedef boost::function< void( enum client_error_codes_enum, boost::system::error_code ) >	on_error_type;

	// STATE[INLINED]: recovered from network::tcp_packet_client::create_client.
	inline	void						set_on_connected			( on_connected_type const& value ) { m_on_connected = value; }
	inline	void						set_on_disconnected			( on_disconnected_type const& value ) { m_on_disconnected = value; }
	inline	void						set_on_packet_received		( on_packet_received_type const& value ) { m_packet_socket.set_on_packet_received( value ); }
	inline	void						set_on_error				(
											on_error_type const&	value
										) { m_on_error = value; }

	inline	boost::asio::io_service&	io_service					( ) { return m_io_service; }

private:
			void						on_connected				( );
			void						close_connection			( );
			void						on_error					( client_error_codes_enum client_error_code, boost::system::error_code error_code );
			void						start_reading				( );

private:
	typedef boost::asio::ip::tcp::socket		socket_type;

	/* 0x0000 */	socket_type												m_socket;
	/* 0x0048 */	tcp_packet_socket< socket_type >							m_packet_socket;
	/* 0x04a0 */	async_connector											m_async_connector;
	/* 0x0900 */	on_connected_type										m_on_connected;
	/* 0x0920 */	on_disconnected_type									m_on_disconnected;
	/* 0x0940 */	on_packet_received_type									m_on_packet_received;
	/* 0x0960 */	on_error_type											m_on_error;
	/* 0x0980 */	boost::asio::io_service&									m_io_service;
	/* 0x0984 */	tcp_packet*												m_first_packet;
}; // class tcp_packet_client

STATIC_SIZE_ASSERT(tcp_packet_client, 0x988);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED
