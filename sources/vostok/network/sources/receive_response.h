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
	public boost::noncopyable
{
public:
	typedef boost::function< void ( network_core::packet_reader& ) >	on_packet_received_type;

	// Target emits this constructor only inline in tcp_packet_client::on_packet_received.
	inline			receive_response	(
			on_packet_received_type const& receiver,
			memory::doug_lea_allocator& allocator,
			network_core::tcp_packet const& packet
		) :
		m_receiver				( receiver ),
		m_allocator				( allocator ),
		m_packet				( packet )
	{
	}

	// claude@NOTE: statements byte-equal; residual is the folded function1::clear's
	// LTCG this-convention (esi in target, ecx in base) and the 8-byte frame slack
	// riding on it - not source-steerable.
	virtual			~receive_response	( )
	{
		network_core::tcp_packet const* temp	= &m_packet;
		VOSTOK_DELETE_IMPL		( m_allocator, temp );
	}

	// claude@NOTE: structure matches 2/2; residual is base_packet::buffer() inlined
	// inside the packet_reader-ctor expansion where the target calls the COMDAT -
	// the inline-vs-call wall, not source-steerable.
	virtual	void	execute				( )
	{
		network_core::packet_reader	reader( m_packet );
		m_receiver				( reader );
	}

private:
	on_packet_received_type		m_receiver;
	memory::doug_lea_allocator&	m_allocator;
	network_core::tcp_packet const&		m_packet;
}; // class receive_response

STATIC_SIZE_ASSERT(receive_response, 0x30);

} // namespace network
} // namespace vostok

#endif // #ifndef RECEIVE_RESPONSE_H_INCLUDED
