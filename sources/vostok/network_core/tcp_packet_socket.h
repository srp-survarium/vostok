////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED

namespace vostok {
namespace network_core {

/* vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > > */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class boost::function<void(tcp_packet const &)>;
class handler_allocator;
class boost::function<void(enum client_error_codes_enum,boost::system::error_code)>;
class boost::asio::ip::tcp::socket;
class memory::base_allocator;
class core::noncopyable;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class tcp_packet_socket<boost::asio::ip::tcp::socket > public : core::noncopyable {
public:
	inline										tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >( boost::asio::ip::tcp::socket& arg_0, memory::base_allocator& arg_1 ) { /* no source */ }

			void								send							( tcp_packet const& packet );

	inline	boost::asio::ip::tcp::socket&		next_layer						( ) const { /* no source */ }

	inline	boost::asio::ip::tcp::socket&		lowest_layer					( ) const { /* no source */ }

			void								start_receiving					( );

			void								stop_receiving					( );

	inline	void								set_on_packet_received			( boost::function<void(tcp_packet const &)> const& arg_0 ) { /* no source */ }

	inline	void								set_on_error					( boost::function<void(enum client_error_codes_enum,boost::system::error_code)> const& arg_0 ) { /* no source */ }

			void								on_packet_received				( tcp_packet const* packet, boost::system::error_code const& error_code, u32 bytes_transferred );

			void								on_packet_has_been_sent			( tcp_packet const* packet_being_sent, boost::system::error_code const& error_code, u32 bytes_transferred );

			tcp_packet*							new_packet						( );

	inline	void								delete_packet					( tcp_packet const*& arg_0 ) { /* no source */ }

	inline										~tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >( ) { /* no source */ }


private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	boost::function<void(tcp_packet const &)>	m_on_packet_received;
	/* 0x0020 */	boost::function<void(enum client_error_codes_enum,boost::system::error_code)>	m_on_error;
	/* 0x0040 */	handler_allocator					m_allocator;
	/* 0x0448 */	memory::base_allocator&				m_packet_allocator;
	/* 0x044c */	boost::asio::ip::tcp::socket&		m_socket;
	/* 0x0450 */	u32									m_header_buffer;
}; // class tcp_packet_socket<boost::asio::ip::tcp::socket >

STATIC_SIZE_ASSERT(tcp_packet_socket<boost::asio::ip::tcp::socket >, 0x458);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_H_INCLUDED
