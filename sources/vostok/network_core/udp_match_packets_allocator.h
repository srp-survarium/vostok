////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
#define UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// vostok::network_core::udp_match_packets_allocator::udp_match_packets_allocator(vostok::memory::base_allocator&, void*, unsigned int)
udp_match_packets_allocator::udp_match_packets_allocator( memory::base_allocator& allocator, void* arena, u32 arena_size )
{
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

#endif // #ifndef UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
