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
	// Target emits this constructor only inline in match_client::connect.
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

	// claude@NOTE: statements/slots verified (two same-named PDB locals = two
	// disjoint braced scopes, [ebp-4]/[ebp-8]); structure matches 4/4. Residual is
	// base inlining intrusive_ptr<udp_match_packets_allocator>::operator* and the
	// member ~intrusive_ptr where the target keeps out-of-line calls (the inlined
	// operator* body carries intrusive_ptr_inline.h's compiled-out ASSERT byte) -
	// the per-call-site inline-vs-call wall, not source-steerable.
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
