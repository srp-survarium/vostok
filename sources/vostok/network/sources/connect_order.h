////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CONNECT_ORDER_H_INCLUDED
#define CONNECT_ORDER_H_INCLUDED

#include "order.h"
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/udp_match_packets_allocator.h>

namespace vostok {
namespace network {

class connect_order : public order {
public:
	typedef network_core::udp_match_packet			udp_match_packet;
	typedef network_core::udp_match_packets_allocator_ptr	udp_match_packets_allocator_ptr;
	typedef boost::function< void ( pcstr, udp_match_packet const* ) >	connect_type;

	// Target emits this constructor only inline in match_client::connect.
	inline			connect_order	(
			memory::base_allocator& strings_allocator,
			pcstr const host,
			udp_match_packets_allocator_ptr const& packets_allocator,
			udp_match_packet const* packet,
			connect_type const& connector
		) :
		m_connector			( connector ),
		m_packets_allocator	( packets_allocator ),
		m_host				( strings::duplicate( strings_allocator, host ) ),
		m_strings_allocator	( strings_allocator ),
		m_packet			( packet )
	{
	}

	virtual			~connect_order	( )
#line 39
	{
		{
			pstr temp			= m_host;
			VOSTOK_FREE_IMPL	( m_strings_allocator, temp );
		}
		{
			udp_match_packet* temp	= const_cast< udp_match_packet* >( m_packet );
#line 47
			network_core::delete_udp_match_packet	( *m_packets_allocator, temp );
		}
	}

	virtual	void	execute			( )
	{
		m_connector			( m_host, m_packet );
	}

private:
	const connect_type			m_connector;
	const udp_match_packets_allocator_ptr	m_packets_allocator;
	char* const					m_host;
	memory::base_allocator&		m_strings_allocator;
	udp_match_packet const* const	m_packet;
}; // class connect_order

STATIC_SIZE_ASSERT(connect_order, 0x38);

} // namespace network
} // namespace vostok

#endif // #ifndef CONNECT_ORDER_H_INCLUDED
