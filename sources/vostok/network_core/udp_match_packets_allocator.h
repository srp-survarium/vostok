// SPDX-License-Identifier: GPL-3.0-or-later

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
	}

	// STATE[REMOVED] (destroy/increment/decrement): the intrusive_ptr machinery reads
	// object.m_reference_count directly through threading::multi_threading_policy (see
	// threading_policies.h) - it never calls these member hooks, and no other consumer
	// does. The only emitted/target member is the ctor. Uninstantiated both sides.
	inline	void	destroy						( udp_match_packets_allocator* allocator ) { /* no source */ } // STATE[REMOVED]

	inline	void	increment					( ) { /* no source */ } // STATE[REMOVED]
	inline	void	decrement					( ) { /* no source */ } // STATE[REMOVED]

private:
	// intrusive_ptr's threading policy reads m_reference_count directly
	friend class threading::multi_threading_policy;

	/* 0x000c */	memory::base_allocator&		m_allocator;
	// volatile (threading::atomic32_type): the interlocked guard requires it
	/* 0x0010 */	threading::atomic32_type	m_reference_count;
}; // class udp_match_packets_allocator

STATIC_SIZE_ASSERT(udp_match_packets_allocator, 0x14);

// refcounted handle held by the network module's match_client/connect_order
// (PDB: vostok::network_core::udp_match_packets_allocator_ptr)
typedef intrusive_ptr<
	udp_match_packets_allocator,
	udp_match_packets_allocator,
	threading::multi_threading_policy
>	udp_match_packets_allocator_ptr;

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
