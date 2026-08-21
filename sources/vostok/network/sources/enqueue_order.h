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
	// Target emits this constructor only inline in match_client::enqueue.
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

	// claude@NOTE: statements/slots verified (temp local [ebp-4], same 0x50 frame);
	// structure matches 4/4. Residual is base inlining
	// intrusive_ptr<udp_match_packets_allocator>::operator* and the member
	// ~intrusive_ptr where the target keeps out-of-line calls - same per-call-site
	// inline-vs-call wall as ~connect_order, not source-steerable.
	virtual			~enqueue_order	( )
	{
		network_core::udp_match_packet* temp	= &m_packet;

		network_core::delete_udp_match_packet	( *m_allocator, temp );

		if ( m_copied_stats >= m_target_stats )
			m_target_stats	= m_copied_stats;
	}

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
