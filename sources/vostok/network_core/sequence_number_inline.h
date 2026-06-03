////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef SEQUENCE_NUMBER_INLINE_H_INCLUDED
#define SEQUENCE_NUMBER_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// int vostok::network_core::operator-<unsigned short>(vostok::network_core::sequence_number<unsigned short> const&, vostok::network_core::sequence_number<unsigned short> const&)
s32 operator-<u16>( sequence_number<u16> const& left, sequence_number<u16> const& right )
{
	return 0;
	// FUNCTION BODY[0x137b20]: 4
	// <0x137b23>|0x000|0x000:'122'
	// <0x137b36>|0x013|0x013:'123'
	// 1
	// <0x137b59>|0x036|0x023:'125'
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
		boost::function<void __cdecl(udp_match_client_session &,u8,packet_reader &)>
		on_packet_received_type;

	typedef
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_client_session,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,1372>,udp_match_server::comparer,u32,1> >
		tree_type;

	typedef
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_packet,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,8>,udp_match_connection::comparer,u32,1> >
		tree_type;

	typedef
		sockaddr
		data_type;

	typedef
		udp_match_packet**
		iterator_type;

	// ******

} // namespace network_core
} // namespace vostok

#endif // #ifndef SEQUENCE_NUMBER_INLINE_H_INCLUDED
