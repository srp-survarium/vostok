////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef SEND_ORDER_H_INCLUDED
#define SEND_ORDER_H_INCLUDED

#include "order.h"
#include <vostok/network_core/tcp_packet.h>

namespace vostok {
namespace network {

class send_order : public order {
public:
	typedef boost::function< void ( network_core::tcp_packet const& ) >	send_type;

	// STATE[INLINED]: no standalone target symbol (pdb_rich_query lists only
	// dtor/execute/??_G); the only construction site is tcp_packet_client::send
	inline				send_order	(
			send_type const& sender,
			network_core::tcp_packet const& packet,
			memory::base_allocator& allocator
		) :
		m_sender			( sender ),
		m_packet			( packet ),
		m_allocator			( allocator )
	{
	}

	// STATE[100%|DONE]
	virtual				~send_order	( )
	{
		network_core::tcp_packet const* temp	= &m_packet;
		VOSTOK_DELETE_IMPL	( m_allocator, temp );
	}

	// STATE[100%|DONE]
	virtual	void		execute		( )
	{
		m_sender			( m_packet );
	}

private:
	send_type				m_sender;
	network_core::tcp_packet const&		m_packet;
	memory::base_allocator&	m_allocator;
}; // class send_order

STATIC_SIZE_ASSERT(send_order, 0x30);

} // namespace network
} // namespace vostok

#endif // #ifndef SEND_ORDER_H_INCLUDED
