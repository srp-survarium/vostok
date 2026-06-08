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

class tcp_packet : public packet< tcp_packet >, public boost::noncopyable {
public:
	inline	explicit	tcp_packet		( memory::base_allocator& allocator );
	inline				~tcp_packet		( );

	inline	u32			allocated_size	( ) const;

	inline	void		reallocate		( u32 new_size );

private:
	/* 0x0008 */	memory::base_allocator&		m_allocator;
	/* 0x000c */	u32							m_allocated_size;
}; // class tcp_packet

STATIC_SIZE_ASSERT(tcp_packet, 0x10);

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/tcp_packet_inline.h>

#endif // #ifndef NETWORK_CORE_TCP_PACKET_H_INCLUDED
