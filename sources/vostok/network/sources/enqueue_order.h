////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ENQUEUE_ORDER_H_INCLUDED
#define ENQUEUE_ORDER_H_INCLUDED

#include "order.h"
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packets_allocator.h>
#include <vostok/network_core/udp_match_packet.h>

namespace vostok {
namespace network {

class enqueue_order : public order {
public:
	// STATE[INLINED]: no standalone symbol; init list verified against the inline
	// expansion in match_client::enqueue (target rva 0x74cf90, +0xeb..+0x166):
	// m_copied_stats rep-movsd copy FROM target_stats (this+8 of the caller, the
	// same address stored to m_target_stats), enqueuer function1 copy, packet
	// address, allocator intrusive_ptr copy-ctor, source/target reference stores
	inline			enqueue_order	(
			boost::function< void ( network_core::udp_match_packet& ) > const& enqueuer,
			network_core::udp_match_packet& packet,
			network_core::udp_match_packets_allocator_ptr const& allocator,
			network_core::udp_match_stats const& source_stats,
			network_core::udp_match_stats& target_stats
		) :
		m_copied_stats	( target_stats ),
		m_enqueuer		( enqueuer ),
		m_packet		( packet ),
		m_allocator		( allocator ),
		m_source_stats	( source_stats ),
		m_target_stats	( target_stats )
	{
	}

	// STATE[63.83%|PARTIAL]: statements/slots verified (temp local [ebp-4], same
	// 0x50 frame); residual = base INLINES
	// intrusive_ptr<udp_match_packets_allocator>::operator* and the member
	// ~intrusive_ptr where the target keeps out-of-line calls (per-call-site
	// whole-program LTCG inline-vs-call, same wall as ~connect_order; the inlined
	// operator* body carries intrusive_ptr_inline.h's compiled-out ASSERT byte)
	virtual			~enqueue_order	( )
	{
		network_core::udp_match_packet* temp	= &m_packet;

		network_core::delete_udp_match_packet	( *m_allocator, temp );

		if ( m_copied_stats >= m_target_stats )
			m_target_stats	= m_copied_stats;

		// STRUCTURE DIFF: target 4 stmts / base 4 stmts (SIZE-only)
		// VERDICT: STRUCTURE MATCH - same intrusive_ptr operator*/~intrusive_ptr inline-vs-call as ~connect_order; non-steerable LTCG.
	}

	// STATE[100%|DONE]
	virtual	void	execute			( )
	{
		m_enqueuer			( m_packet );
		ASSERT				( UNKNOWN_EXPRESSION_T( m_source_stats >= m_copied_stats ) );
		m_copied_stats		= m_source_stats;
	}

private:
	network_core::udp_match_stats		m_copied_stats;
	boost::function< void ( network_core::udp_match_packet& ) >	m_enqueuer;
	network_core::udp_match_packet&		m_packet;
	network_core::udp_match_packets_allocator_ptr	m_allocator;
	network_core::udp_match_stats const&	m_source_stats;
	network_core::udp_match_stats&		m_target_stats;
}; // class enqueue_order

STATIC_SIZE_ASSERT(enqueue_order, 0xB8);

} // namespace network
} // namespace vostok

#endif // #ifndef ENQUEUE_ORDER_H_INCLUDED
