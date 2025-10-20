////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED

namespace vostok {
namespace network_core {

/* vostok::network_core::tcp_packet_client */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class tcp_packet_socket<boost::asio::ip::tcp::socket >;
class boost::function<void(tcp_packet const &)>;
class async_connector;
class boost::function<void(enum client_error_codes_enum,boost::system::error_code)>;
class boost::asio::ip::tcp::socket;
class boost::asio::io_service;
class tcp_packet;
class boost::function<void()>;
class boost::noncopyable;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class tcp_packet_client public : boost::noncopyable {
public:
										tcp_packet_client			( boost::asio::io_service& io_service );

										~tcp_packet_client			( );

			void						connect						( pcstr host, u16 port );

			void						disconnect					( );

			void						send						( tcp_packet const& packet );

	inline	bool						is_connected				( ) const { /* no source */ }

	inline	bool						has_connection_established	( ) const { /* no source */ }

	inline	void						set_on_connected			( boost::function<void()> const& arg_0 ) { /* no source */ }

	inline	void						set_on_disconnected			( boost::function<void()> const& arg_0 ) { /* no source */ }

	inline	void						set_on_packet_received		( boost::function<void(tcp_packet const &)> const& arg_0 ) { /* no source */ }

	inline	void						set_on_error				( boost::function<void(enum client_error_codes_enum,boost::system::error_code)> const& arg_0 ) { /* no source */ }

	inline	boost::asio::io_service&	io_service					( ) { /* no source */ }

			void						on_connected				( );

			void						close_connection			( );

			void						on_error					( client_error_codes_enum client_error_code, boost::system::error_code error_code );

			void						start_reading				( );


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	boost::asio::ip::tcp::socket		m_socket;
	/* 0x0048 */	tcp_packet_socket<boost::asio::ip::tcp::socket >	m_packet_socket;
	/* 0x04a0 */	async_connector						m_async_connector;
	/* 0x0900 */	boost::function<void()>				m_on_connected;
	/* 0x0920 */	boost::function<void()>				m_on_disconnected;
	/* 0x0940 */	boost::function<void(tcp_packet const &)>	m_on_packet_received;
	/* 0x0960 */	boost::function<void(enum client_error_codes_enum,boost::system::error_code)>	m_on_error;
	/* 0x0980 */	boost::asio::io_service&			m_io_service;
	/* 0x0984 */	tcp_packet*							m_first_packet;
}; // class tcp_packet_client

STATIC_SIZE_ASSERT(tcp_packet_client, 0x988);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_CLIENT_H_INCLUDED
