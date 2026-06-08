////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

template < typename Socket >
inline void tcp_packet_socket< Socket >::on_packet_received(
	tcp_packet const*					packet,
	boost::system::error_code const&	error_code,
	u32									bytes_transferred
)
{
	// FUNCTION BODY[0x134f20]: 28
	// <0x134f37>|0x017|+0x015:'16'
	// <0x134f4c>|0x02c|+0x057:'17'
	// <0x134fa3>|0x083|+0x005:'18'
	// <0x134fa8>|0x088|+0x0bc:'20'
	// <0x135064>|0x144|+0x01f:'21'
	// <0x135083>|0x163|+0x01a:'22'
	// <0x13509d>|0x17d|+0x039:'24'
	// <0x1350d6>|0x1b6|+0x005:'25'
	// <0x1350db>|0x1bb|+0x011:'28'
	// <0x1350ec>|0x1cc|+0x077:'29'
	// <0x135163>|0x243|+0x01e:'30'
	// <0x135181>|0x261|+0x01a:'31'
	// <0x13519b>|0x27b|+0x039:'33'
	// <0x1351d4>|0x2b4|+0x002:'34'
	// <0x1351d6>|0x2b6|+0x01b:'39'
	// <0x1351f1>|0x2d1|+0x00f:'40'
	// <0x135200>|0x2e0|+0x039:'42'
	// <0x135239>|0x319|+0x00b:'43'
	// ******
}

template < typename Socket >
template < typename T >
inline void tcp_packet_socket< Socket >::on_packet_size_received( boost::system::error_code const& error_code, u32 bytes_transferred )
{
	// LOCALS
	// const u16 						buffer_size
	// tcp_packet* const 				packet
	// ******

	// FUNCTION BODY[0x135370]: 55 : on_packet_size_received< u16 >
	// <0x13538a>|0x01a|+0x015:'50'
	// <0x13539f>|0x02f|+0x069:'51'
	// <0x135408>|0x098|+0x005:'52'
	// <0x13540d>|0x09d|+0x0da:'54'
	// <0x1354e7>|0x177|+0x01f:'55'
	// <0x135506>|0x196|+0x01a:'56'
	// <0x135520>|0x1b0|+0x005:'57'
	// <0x135525>|0x1b5|+0x00a:'60'
	// <0x13552f>|0x1bf|+0x081:'61'
	// <0x1355b0>|0x240|+0x01f:'62'
	// <0x1355cf>|0x25f|+0x01a:'63'
	// <0x1355e9>|0x279|+0x005:'64'
	// <0x1355ee>|0x27e|+0x011:'67'
	// <0x1355ff>|0x28f|+0x00c:'68'
	// <0x13560b>|0x29b|+0x00e|[1]:'69'
	// <0x135619>|0x2a9|+0x00d:'70'
	// <0x135626>|0x2b6|+0x0a5:'85'
	// <0x1356cb>|0x35b|+0x005:'86'
	// <0x1356d0>|0x360|+0x01d:'89'
	// <0x1356ed>|0x37d|+0x0bc:'102'
	// <0x1357a9>|0x439|+0x002:'103'
	// <0x1357ab>|0x43b|+0x012:'104'
	// ******
}

template < typename Socket >
inline void tcp_packet_socket< Socket >::start_receiving( )
{
	// FUNCTION BODY[0x133fb0]: 13
	// <0x133fbf>|0x00f|+0x08c:'122'
	// ******
}

template < typename Socket >
inline void tcp_packet_socket< Socket >::on_packet_has_been_sent(
	tcp_packet const*					packet_being_sent,
	boost::system::error_code const&	error_code,
	u32									bytes_transferred
)
{
	// FUNCTION BODY[0x1340b0]: 17
	// <0x1340c7>|0x017|+0x02d:'132'
	// <0x1340f4>|0x044|+0x014:'134'
	// <0x134108>|0x058|+0x0bf:'135'
	// <0x1341c7>|0x117|+0x01f:'136'
	// <0x1341e6>|0x136|+0x01a:'137'
	// <0x134200>|0x150|+0x005:'138'
	// <0x134205>|0x155|+0x00a:'141'
	// <0x13420f>|0x15f|+0x07b:'142'
	// <0x13428a>|0x1da|+0x01f:'143'
	// <0x1342a9>|0x1f9|+0x01a:'144'
	// ******
}

template < typename Socket >
inline void tcp_packet_socket< Socket >::send( tcp_packet const& packet )
{
	// LOCALS
	// tcp_packet* 						cloned_packet
	// ******

	// FUNCTION BODY[0x133ee0]: 34
	// <0x133efa>|0x01a|+0x00e:'154'
	// <0x133f08>|0x028|+0x024:'155'
	// <0x133f2c>|0x04c|+0x016:'179'
	// <0x133f42>|0x062|+0x00f:'180'
	// <0x133f51>|0x071|+0x02d:'186'
	// <0x133f7e>|0x09e|+0x020:'187'
	// ******
}

template < typename Socket >
inline tcp_packet* tcp_packet_socket< Socket >::new_packet( )
{
	return NULL;
	// FUNCTION BODY[0x1342d0]: 1
	// <0x1342d9>|0x009|+0x084:'193'
	// ******
}

template < typename Socket >
inline void tcp_packet_socket< Socket >::stop_receiving( )
{
	// FUNCTION BODY[0x134050]: 2
	// <0x13405f>|0x00f|+0x00f:'205'
	// <0x13406e>|0x01e|+0x031:'206'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_SOCKET_INLINE_H_INCLUDED
