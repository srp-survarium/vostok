////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "tcp_packet_client.h"

namespace vostok {
namespace network_core {

// STATE[STUB]
// vostok::network_core::tcp_packet_client::tcp_packet_client(boost::asio::io_service&)
tcp_packet_client::tcp_packet_client( boost::asio::io_service& io_service )
{
	// FUNCTION BODY[0x78cc70]: 1
	// <0x78cd34>|0x000|0x000:'20'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::~tcp_packet_client()
void tcp_packet_client::~tcp_packet_client( )
{
	// FUNCTION BODY[0x78cbe0]: 4
	// <0x78cbea>|0x000|0x000:'25'
	// <0x78cc00>|0x016|0x016:'26'
	// 1
	// <0x78cc02>|0x018|0x002:'28'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::start_reading()
void tcp_packet_client::start_reading( )
{
	// FUNCTION BODY[0x78cde0]: 1
	// <0x78cdef>|0x000|0x000:'33'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::on_connected()
void tcp_packet_client::on_connected( )
{
	// FUNCTION BODY[0x78ce10]: 4
	// <0x78ce1f>|0x000|0x000:'38'
	// <0x78ce40>|0x021|0x021:'39'
	// 1
	// <0x78ce51>|0x032|0x011:'41'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::connect(char const*, unsigned short)
void tcp_packet_client::connect( pcstr host, u16 port )
{
	// FUNCTION BODY[0x78ce60]: 1
	// <0x78ce69>|0x000|0x000:'46'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::disconnect()
void tcp_packet_client::disconnect( )
{
	// FUNCTION BODY[0x78cb90]: 3
	// <0x78cb9f>|0x000|0x000:'51'
	// <0x78cbad>|0x00e|0x00e:'52'
	// <0x78cbd1>|0x032|0x024:'53'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::close_connection()
void tcp_packet_client::close_connection( )
{
	// LOCALS
	// boost::system::error_code 	ec
	// ******

	// FUNCTION BODY[0x78cb10]: 5
	// <0x78cb1f>|0x000|0x000:'58'
	// <0x78cb2b>|0x00c|0x00c:'59'
	// <0x78cb3a>|0x01b|0x00f:'60'
	// <0x78cb69>|0x04a|0x02f:'61'
	// <0x78cb74>|0x055|0x00b:'62'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::send(vostok::network_core::tcp_packet const&)
void tcp_packet_client::send( tcp_packet const& packet )
{
	// FUNCTION BODY[0x78cda0]: 1
	// <0x78cdb9>|0x000|0x000:'67'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_client::on_error(vostok::network_core::client_error_codes_enum, boost::system::error_code)
void tcp_packet_client::on_error( client_error_codes_enum client_error_code, boost::system::error_code error_code )
{
	// FUNCTION BODY[0x78caa0]: 3
	// <0x78caaf>|0x000|0x000:'72'
	// <0x78cac0>|0x011|0x011:'73'
	// <0x78cae1>|0x032|0x021:'74'
	// ******
}

	// TYPEDEFS
	typedef
		boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> >
		socket_type;

	typedef
		boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>
		iterator_type;

	typedef
		boost::asio::stream_socket_service<boost::asio::ip::tcp>
		service_type;

	typedef
		boost::function<void __cdecl(enum client_error_codes_enum,boost::system::error_code)>
		on_error_type;

	typedef
		sockaddr
		data_type;

	// ******

} // namespace network_core
} // namespace vostok
