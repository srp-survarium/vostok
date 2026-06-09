////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SEND_QUEUED_ORDER_H_INCLUDED
#define SEND_QUEUED_ORDER_H_INCLUDED

#include "order.h"
#include <vostok/network_core/udp_match_stats.h>

namespace vostok {
namespace network {

class match_client_impl;

class send_queued_order : public order {
public:
	// STATE[STUB]: ctor is fully inlined into its caller (no standalone carcass);
	// m_copied_stats default-init is a buildability guess - the matcher confirms
	inline			send_queued_order	(
			boost::function< void ( ) > const& functor,
			match_client_impl*& client,
			network_core::udp_match_stats& target_stats
		) :
		m_functor		( functor ),
		m_target_stats	( target_stats ),
		m_client		( client )
	{
	}

	// STATE[STUB]
	virtual			~send_queued_order	( )
	{
		// FUNCTION BODY[0xeafe0]: 2
		// <0xeaff4>|0x014|+0x020:'37'
		// <0xeb014>|0x034|+0x016:'38'
		// ******
	}

	// STATE[STUB]
	virtual	void	execute				( )
	{
		// LOCALS
		// network_core::udp_match_stats const& source_stats
		// ******

		// FUNCTION BODY[0xeb060]: 5
		// <0xeb071>|0x011|+0x011:'43'
		// <0>
		// <0xeb082>|0x022|+0x017:'45'
		// <0xeb099>|0x039|+0x00c:'46'
		// <0xeb0a5>|0x045|+0x013:'47'
		// ******
	}

private:
	network_core::udp_match_stats		m_copied_stats;
	boost::function< void ( ) >			m_functor;
	network_core::udp_match_stats&		m_target_stats;
	match_client_impl*&					m_client;
}; // class send_queued_order

STATIC_SIZE_ASSERT(send_queued_order, 0xB0);

} // namespace network
} // namespace vostok

#endif // #ifndef SEND_QUEUED_ORDER_H_INCLUDED
