////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
#define NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace network_core {

class boost::aligned_storage<1024,4294967295>;
class core::noncopyable;

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
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_packet,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,8>,udp_match_connection::comparer,u32,1> >
		tree_type;

	typedef
		sockaddr
		data_type;

	// ******

class handler_allocator public : core::noncopyable {
public:
						handler_allocator	( );

	inline	void*		allocate			( u32 arg_0 ) { /* no source */ }
	inline	void		deallocate			( void* arg_0 ) { /* no source */ }

	inline				~handler_allocator	( ) { /* no source */ }

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	boost::aligned_storage<1024,4294967295>	storage_;
	/* 0x0400 */	bool								in_use_;
}; // class handler_allocator

STATIC_SIZE_ASSERT(handler_allocator, 0x408);


// STATE[STUB]
// vostok::network_core::handler_allocator::handler_allocator()
handler_allocator::handler_allocator( )
{
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED