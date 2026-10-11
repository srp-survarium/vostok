////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef RECEIVE_RESPONSE_H_INCLUDED
#define RECEIVE_RESPONSE_H_INCLUDED

#include "response.h"
#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network {

class receive_response :
	public response,
	private boost::noncopyable
{
public:
	typedef boost::function< void ( network_core::packet_reader& ) >	on_packed_received_type;

	// Target emits this constructor only inline in tcp_packet_client::on_packet_received.
	inline			receive_response	(
			on_packed_received_type const& receiver,
			memory::doug_lea_allocator& allocator,
			network_core::tcp_packet const& packet
		) :
		m_receiver				( receiver ),
		m_allocator				( allocator ),
		m_packet				( packet )
	{
	}




	virtual			~receive_response	( )
#line 36
	{
		network_core::tcp_packet const* temp	= &m_packet;
		VOSTOK_DELETE_IMPL		( m_allocator, temp );
	}




	virtual	void	execute				( )
#line 42
	{
		network_core::packet_reader	reader( m_packet );
		m_receiver				( reader );
	}

private:
	on_packed_received_type		m_receiver;
	memory::doug_lea_allocator&	m_allocator;
	network_core::tcp_packet const&		m_packet;
}; // class receive_response

STATIC_SIZE_ASSERT(receive_response, 0x30);

} // namespace network
} // namespace vostok

#endif // #ifndef RECEIVE_RESPONSE_H_INCLUDED
