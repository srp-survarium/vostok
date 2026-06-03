////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
#define UDP_MATCH_CONNECTION_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// static void vostok::network_core::udp_match_connection::construct_packet(vostok::network_core::udp_match_packets_orderer&, vostok::network_core::udp_match_packet&, unsigned char)
static void udp_match_connection::construct_packet( udp_match_packets_orderer& packets_orderer, udp_match_packet& packet, u8 message_type )
{
	// LOCALS
	// udp_match_message_type_info const& info
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xd9cbf }, type_index: TypeIndex(0x1906e) })
	// ******

	// FUNCTION BODY[0xeac90]: 14
	// <0xeac96>|0x000|0x000:'22'
	// <0xeac9f>|0x009|0x009:'23'
	// <0xeacac>|0x016|0x00d:'24'
	// 1
	// <0xeacc7>|0x031|0x01b:'26'
	// <0xeace3>|0x04d|0x01c:'27'
	// <0xead04>|0x06e|0x021:'28'
	// 1
	// <0xead24>|0x08e|0x020:'30'
	// <0xead37>|0x0a1|0x013:'31'
	// 1
	// 2
	// 3
	// 4
	// ******
}

	// TYPEDEFS
	typedef
		boost::asio::basic_socket<boost::asio::ip::udp,boost::asio::datagram_socket_service<boost::asio::ip::udp> >
		lowest_layer_type;

	typedef
		boost::asio::datagram_socket_service<boost::asio::ip::udp>
		service_type;

	typedef
		boost::asio::ip::basic_endpoint<boost::asio::ip::udp>
		endpoint_type;

	typedef
		boost::asio::ip::udp
		protocol_type;

	typedef
		boost::function<void __cdecl(u8,packet_reader &)>
		client_on_packet_received_type;

	typedef
		boost::function<void __cdecl(u8,packet_reader &)>
		on_packet_received_type;

	typedef
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_packet,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,8>,udp_match_connection::comparer,u32,1> >
		tree_type;

	typedef
		sockaddr
		data_type;

	// ******

} // namespace network_core
} // namespace vostok

#endif // #ifndef UDP_MATCH_CONNECTION_INLINE_H_INCLUDED
