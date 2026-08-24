////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_H_INCLUDED

#include <vostok/network_core/packet.h>
#include <boost/noncopyable.hpp>

namespace vostok {

namespace memory {
	class base_allocator;
} // namespace memory

namespace network_core {

class tcp_packet : public packet< tcp_packet >, private boost::noncopyable {
	friend class packet< tcp_packet >;

public:
	inline	explicit	tcp_packet		( memory::base_allocator& allocator );
	inline				~tcp_packet		( );

	inline	u32			allocated_size	( ) const;

private:
	inline	void		reallocate		( u32 new_size );

	/* 0x0008 */	memory::base_allocator&		m_allocator;
	/* 0x000c */	u32							m_allocated_size;
}; // class tcp_packet

STATIC_SIZE_ASSERT(tcp_packet, 0x10);

// only visible under asio (their network_core consumers always include it);
// lets asio-free TUs (game's match_options/relocate_item_descr serialize bodies)
// use tcp_packet::append without dragging asio under a 0x0500 pch
#ifdef BOOST_ASIO_HPP
boost::asio::const_buffers_1	buffer_to_send			( tcp_packet& packet );
boost::asio::mutable_buffers_1	buffer_to_receive_into	( tcp_packet& packet );
#endif // #ifdef BOOST_ASIO_HPP

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/tcp_packet_inline.h>

#endif // #ifndef NETWORK_CORE_TCP_PACKET_H_INCLUDED
