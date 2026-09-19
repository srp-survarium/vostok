// SPDX-License-Identifier: GPL-3.0-or-later

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
	typedef network_core::udp_match_packet			udp_match_packet;
	typedef network_core::udp_match_stats			udp_match_stats;
	typedef network_core::udp_match_packets_allocator_ptr	udp_match_packets_allocator_ptr;
	typedef boost::function< void ( udp_match_packet& ) >	enqueue_type;

	// Target emits this constructor only inline in match_client::enqueue.
	inline			enqueue_order	(
			enqueue_type const& enqueuer,
			udp_match_packet& packet,
			udp_match_packets_allocator_ptr const& allocator,
			udp_match_stats const& source_stats,
			udp_match_stats& target_stats
		) :
		m_copied_stats	( target_stats ),
		m_enqueuer		( enqueuer ),
		m_packet		( packet ),
		m_allocator		( allocator ),
		m_source_stats	( source_stats ),
		m_target_stats	( target_stats )
	{
	}

	virtual			~enqueue_order	( )
#line 41
	{
		udp_match_packet* temp	= &m_packet;

		network_core::delete_udp_match_packet	( *m_allocator, temp );

#line 47
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
	udp_match_stats			m_copied_stats;
	enqueue_type			m_enqueuer;
	udp_match_packet&		m_packet;
	udp_match_packets_allocator_ptr	m_allocator;
	udp_match_stats const&	m_source_stats;
	udp_match_stats&		m_target_stats;
}; // class enqueue_order

STATIC_SIZE_ASSERT(enqueue_order, 0xB8);

} // namespace network
} // namespace vostok

#endif // #ifndef ENQUEUE_ORDER_H_INCLUDED
