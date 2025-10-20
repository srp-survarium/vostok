////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_H_INCLUDED

#include <vostok/network_core/api.h>
#include <vostok/network_core/packet.h>


namespace vostok {
namespace network_core {

class memory::base_allocator;


class tcp_packet : public packet<tcp_packet>, public boost::noncopyable {
public:
	inline	explicit	tcp_packet		( memory::base_allocator& allocator );
						~tcp_packet		( );

	inline	u32			allocated_size	( ) const { return m_allocated_size; }

	inline	void		reallocate		( u32 new_size ) { /* no source */ }

private:
	/* 0x0000 */	/* packet<tcp_packet> */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	memory::base_allocator&		m_allocator;
	/* 0x000c */	u32							m_allocated_size;
}; // class tcp_packet

STATIC_SIZE_ASSERT(tcp_packet, 0x10);

// sushi@TODO: Should be private in packet_socket.h
boost::asio::const_buffers_1	buffer_to_send			( tcp_packet& packet );
boost::asio::mutable_buffers_1	buffer_to_receive_into	( tcp_packet& packet );

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/tcp_packet_inline.h>

#endif // #ifndef NETWORK_CORE_TCP_PACKET_H_INCLUDED