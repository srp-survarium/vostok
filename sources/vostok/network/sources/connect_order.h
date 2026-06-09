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
	// STATE[STUB]: ctor is fully inlined into its caller (no standalone carcass);
	// init-list follows member declaration order
	inline			connect_order	(
			memory::base_allocator& strings_allocator,
			pcstr const host,
			network_core::udp_match_packets_allocator_ptr const& packets_allocator,
			network_core::udp_match_packet const* const packet,
			boost::function< void ( pcstr, network_core::udp_match_packet const* ) > const& connector
		) :
		m_connector			( connector ),
		m_packets_allocator	( packets_allocator ),
		m_host				( strings::duplicate( strings_allocator, host ) ),
		m_strings_allocator	( strings_allocator ),
		m_packet			( packet )
	{
	}

	// STATE[PARTIAL]: legacy body ported onto canonical types; unverified vs target
	// FUNCTION BODY[0xeaee0]
	virtual			~connect_order	( )
	{
		{
			pstr temp			= m_host;
			VOSTOK_FREE_IMPL	( m_strings_allocator, temp );
		}
		{
			network_core::udp_match_packet* temp	= const_cast< network_core::udp_match_packet* >( m_packet );
			network_core::delete_udp_match_packet	( *m_packets_allocator, temp );
		}
	}

	// STATE[PARTIAL]: legacy body ported onto canonical types; unverified vs target
	// FUNCTION BODY[0xeaf70]
	virtual	void	execute			( )
	{
		m_connector			( m_host, m_packet );
	}

private:
	const boost::function< void ( pcstr, network_core::udp_match_packet const* ) >	m_connector;
	const network_core::udp_match_packets_allocator_ptr	m_packets_allocator;
	char* const					m_host;
	memory::base_allocator&		m_strings_allocator;
	network_core::udp_match_packet const* const				m_packet;
}; // class connect_order

STATIC_SIZE_ASSERT(connect_order, 0x38);

} // namespace network
} // namespace vostok

#endif // #ifndef CONNECT_ORDER_H_INCLUDED
