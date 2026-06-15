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

	// STATE[INLINED]: no standalone symbol on either side; init list verified
	// against the inline expansion in tcp_packet_client::on_packet_received
	// (target rva 0x74bef0): receiver function1 copy-ctor, allocator + packet
	// reference stores
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

	// STATE[93.94%|PARTIAL]: statements byte-equal; residual is the folded
	// function1::clear's LTCG convention (this in ESI in target, ECX in base) and
	// the 8-byte frame slack riding on it
	virtual			~receive_response	( )
	{
		network_core::tcp_packet const* temp	= &m_packet;
		VOSTOK_DELETE_IMPL		( m_allocator, temp );
	}

	// STATE[76.65%|PARTIAL]: base inlines base_packet::buffer() inside the
	// packet_reader-ctor expansion where the target calls the COMDAT (the
	// single-TU-anchor LTCG wall, see assembly_patterns.md); rest byte-equal
	virtual	void	execute				( )
	{
		network_core::packet_reader	reader( m_packet );
		m_receiver				( reader );

		// STRUCTURE DIFF: target 2 stmts / base 2 stmts (SIZE-only)
		// VERDICT: STRUCTURE MATCH - base_packet::buffer() inlined in the packet_reader-ctor expansion (target calls COMDAT); non-steerable LTCG.
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
