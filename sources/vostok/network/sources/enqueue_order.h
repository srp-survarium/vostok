////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ENQUEUE_ORDER_H_INCLUDED
#define ENQUEUE_ORDER_H_INCLUDED

#include "order.h"
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packets_allocator.h>

namespace vostok {

namespace network_core {
	class udp_match_packet;
} // namespace network_core

namespace network {

class enqueue_order : public order {
public:
	// STATE[STUB]: ctor is fully inlined into its caller (no standalone carcass);
	// m_copied_stats source is a buildability guess - the matcher confirms
	inline			enqueue_order	(
			boost::function< void ( network_core::udp_match_packet& ) > const& enqueuer,
			network_core::udp_match_packet& packet,
			network_core::udp_match_packets_allocator_ptr const& allocator,
			network_core::udp_match_stats const& source_stats,
			network_core::udp_match_stats& target_stats
		) :
		m_copied_stats	( source_stats ),
		m_enqueuer		( enqueuer ),
		m_packet		( packet ),
		m_allocator		( allocator ),
		m_source_stats	( source_stats ),
		m_target_stats	( target_stats )
	{
	}

	// STATE[STUB]
	virtual			~enqueue_order	( )
	{
		// LOCALS
		// network_core::udp_match_packet* 	temp
		// ******

		// FUNCTION BODY[0xeada0]: 7
		// <0xeadb4>|0x014|+0x00c:'42'
		// <0>
		// <0xeadc0>|0x020|+0x01a:'44'
		// <0>
		// <1>
		// <0xeadda>|0x03a|+0x020:'47'
		// <0xeadfa>|0x05a|+0x016:'48'
		// ******
	}

	// STATE[STUB]
	virtual	void	execute			( )
	{
		// FUNCTION BODY[0xeae50]: 3
		// <0xeae61>|0x011|+0x01e:'53'
		// <0xeae7f>|0x02f|+0x00c:'54'
		// <0xeae8b>|0x03b|+0x01c:'55'
		// ******
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
