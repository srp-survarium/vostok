////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef TCP_PACKET_SOCKET_INLINE_H_INCLUDED
#define TCP_PACKET_SOCKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// void vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::on_packet_received(vostok::network_core::tcp_packet const*, boost::system::error_code const&, unsigned int)
void tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::on_packet_received( tcp_packet const* packet, boost::system::error_code const& error_code, u32 bytes_transferred )
{
	// FUNCTION BODY[0x134f20]: 28
	// <0x134f37>|0x000|0x000:'16'
	// <0x134f4c>|0x015|0x015:'17'
	// <0x134fa3>|0x06c|0x057:'18'
	// 1
	// <0x134fa8>|0x071|0x005:'20'
	// <0x135064>|0x12d|0x0bc:'21'
	// <0x135083>|0x14c|0x01f:'22'
	// 1
	// <0x13509d>|0x166|0x01a:'24'
	// <0x1350d6>|0x19f|0x039:'25'
	// 1
	// 2
	// <0x1350db>|0x1a4|0x005:'28'
	// <0x1350ec>|0x1b5|0x011:'29'
	// <0x135163>|0x22c|0x077:'30'
	// <0x135181>|0x24a|0x01e:'31'
	// 1
	// <0x13519b>|0x264|0x01a:'33'
	// <0x1351d4>|0x29d|0x039:'34'
	// 1
	// 2
	// 3
	// 4
	// <0x1351d6>|0x29f|0x002:'39'
	// <0x1351f1>|0x2ba|0x01b:'40'
	// 1
	// <0x135200>|0x2c9|0x00f:'42'
	// <0x135239>|0x302|0x039:'43'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::on_packet_size_received<unsigned short>(boost::system::error_code const&, const unsigned int)
void tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::on_packet_size_received<u16>( boost::system::error_code const& error_code, u32 bytes_transferred )
{
	// LOCALS
	// u16 							buffer_size
	// tcp_packet* 					packet<1>
	// ******

	// FUNCTION BODY[0x135370]: 55
	// <0x13538a>|0x000|0x000:'50'
	// <0x13539f>|0x015|0x015:'51'
	// <0x135408>|0x07e|0x069:'52'
	// 1
	// <0x13540d>|0x083|0x005:'54'
	// <0x1354e7>|0x15d|0x0da:'55'
	// <0x135506>|0x17c|0x01f:'56'
	// <0x135520>|0x196|0x01a:'57'
	// 1
	// 2
	// <0x135525>|0x19b|0x005:'60'
	// <0x13552f>|0x1a5|0x00a:'61'
	// <0x1355b0>|0x226|0x081:'62'
	// <0x1355cf>|0x245|0x01f:'63'
	// <0x1355e9>|0x25f|0x01a:'64'
	// 1
	// 2
	// <0x1355ee>|0x264|0x005:'67'
	// <0x1355ff>|0x275|0x011:'68'
	// <0x13560b>|0x281|0x00c|[1]:'69'
	// <0x135619>|0x28f|0x00e:'70'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// <0x135626>|0x29c|0x00d:'85'
	// <0x1356cb>|0x341|0x0a5:'86'
	// 1
	// 2
	// <0x1356d0>|0x346|0x005:'89'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// <0x1356ed>|0x363|0x01d:'102'
	// <0x1357a9>|0x41f|0x0bc:'103'
	// <0x1357ab>|0x421|0x002:'104'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::start_receiving()
void tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::start_receiving( )
{
	// FUNCTION BODY[0x133fb0]: 13
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// <0x133fbf>|0x000|0x000:'122'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::on_packet_has_been_sent(vostok::network_core::tcp_packet const*, boost::system::error_code const&, unsigned int)
void tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::on_packet_has_been_sent( tcp_packet const* packet_being_sent, boost::system::error_code const& error_code, u32 bytes_transferred )
{
	// FUNCTION BODY[0x1340b0]: 17
	// <0x1340c7>|0x000|0x000:'132'
	// 1
	// <0x1340f4>|0x02d|0x02d:'134'
	// <0x134108>|0x041|0x014:'135'
	// <0x1341c7>|0x100|0x0bf:'136'
	// <0x1341e6>|0x11f|0x01f:'137'
	// <0x134200>|0x139|0x01a:'138'
	// 1
	// 2
	// <0x134205>|0x13e|0x005:'141'
	// <0x13420f>|0x148|0x00a:'142'
	// <0x13428a>|0x1c3|0x07b:'143'
	// <0x1342a9>|0x1e2|0x01f:'144'
	// 1
	// 2
	// 3
	// 4
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::send(vostok::network_core::tcp_packet const&)
void tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::send( tcp_packet const& packet )
{
	// LOCALS
	// boost::asio::const_buffers_1 const& buffer
	// boost::system::error_code 	error_code
	// tcp_packet* 					cloned_packet
	// ******

	// FUNCTION BODY[0x133ee0]: 34
	// <0x133efa>|0x000|0x000:'154'
	// <0x133f08>|0x00e|0x00e:'155'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// 20
	// 21
	// 22
	// 23
	// <0x133f2c>|0x032|0x024:'179'
	// <0x133f42>|0x048|0x016:'180'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x133f51>|0x057|0x00f:'186'
	// <0x133f7e>|0x084|0x02d:'187'
	// ******
}

// STATE[STUB]
// vostok::network_core::tcp_packet* vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::new_packet()
tcp_packet* tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::new_packet( )
{
	return NULL;
	// FUNCTION BODY[0x1342d0]: 1
	// <0x1342d9>|0x000|0x000:'193'
	// ******
}

// STATE[STUB]
// void vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::stop_receiving()
void tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >::stop_receiving( )
{
	// LOCALS
	// boost::system::error_code 	error_code
	// ******

	// FUNCTION BODY[0x134050]: 2
	// <0x13405f>|0x000|0x000:'205'
	// <0x13406e>|0x00f|0x00f:'206'
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

#endif // #ifndef TCP_PACKET_SOCKET_INLINE_H_INCLUDED
