////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SEND_QUEUED_ORDER_H_INCLUDED
#define SEND_QUEUED_ORDER_H_INCLUDED

#include "order.h"
#include "match_client_impl.h"
#include <vostok/network_core/udp_match_stats.h>

namespace vostok {
namespace network {

class send_queued_order : public order {
public:
	// Target emits this constructor only inline in match_client::send_queued_packets.
	inline			send_queued_order	(
			boost::function< void ( ) > const& functor,
			match_client_impl*& client,
			network_core::udp_match_stats& target_stats
		) :
		m_copied_stats	( target_stats ),
		m_functor		( functor ),
		m_target_stats	( target_stats ),
		m_client		( client )
	{
	}

	virtual			~send_queued_order	( )
	{
		if ( m_copied_stats >= m_target_stats )
			m_target_stats	= m_copied_stats;
	}

	virtual	void	execute				( )
	{
		m_functor			( );

		network_core::udp_match_stats const& source_stats	= m_client->get_stats( );
		ASSERT				( UNKNOWN_EXPRESSION_T( source_stats >= m_copied_stats ) );
		m_copied_stats		= source_stats;
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
