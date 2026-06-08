////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED

#include <vostok/memory_single_size_buffer_allocator.h>

namespace vostok {

namespace memory {
	class base_allocator;
} // namespace memory

namespace network_core {

class udp_match_packets_allocator : public memory::single_size_buffer_allocator< 300, threading::single_threading_policy > {
public:
	inline			udp_match_packets_allocator	( memory::base_allocator& allocator, void* arena, u32 arena_size ) :
		single_size_buffer_allocator< 300, threading::single_threading_policy >( arena, arena_size ),
		m_allocator			( allocator ),
		m_reference_count	( 0 )
	{
		/* no source */
	}

	inline	void	destroy						( udp_match_packets_allocator* allocator ) { /* no source */ }

	inline	void	increment					( ) { /* no source */ }
	inline	void	decrement					( ) { /* no source */ }

	inline			~udp_match_packets_allocator( ) { /* no source */ }

private:
	/* 0x000c */	memory::base_allocator&		m_allocator;
	/* 0x0010 */	long						m_reference_count;
}; // class udp_match_packets_allocator

STATIC_SIZE_ASSERT(udp_match_packets_allocator, 0x14);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
