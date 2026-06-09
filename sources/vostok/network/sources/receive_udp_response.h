////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RECEIVE_UDP_RESPONSE_H_INCLUDED
#define RECEIVE_UDP_RESPONSE_H_INCLUDED

#include "response.h"
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packets_allocator.h>

namespace vostok {

namespace network_core {
	class udp_match_packet;
	class packet_reader;
} // namespace network_core

namespace network {

class receive_udp_response :
	public response,
	public boost::noncopyable
{
public:
	// STATE[STUB]: ctor is fully inlined into its caller (no standalone carcass);
	// m_copied_stats source is a buildability guess - the matcher confirms
	inline			receive_udp_response	(
			boost::function< void ( network_core::packet_reader& ) > const& receiver,
			network_core::udp_match_packets_allocator_ptr const& allocator,
			network_core::udp_match_packet& packet,
			network_core::udp_match_stats const& source_stats,
			network_core::udp_match_stats& target_stats
		) :
		m_copied_stats	( source_stats ),
		m_receiver		( receiver ),
		m_allocator		( allocator ),
		m_packet		( packet ),
		m_target_stats	( target_stats )
	{
	}

	// STATE[STUB]
	virtual			~receive_udp_response	( )
	{
		// LOCALS
		// network_core::udp_match_packet* 	temp
		// ******

		// FUNCTION BODY[0xeb0f0]: 2
		// <0xeb103>|0x013|+0x00c:'44'
		// <0xeb10f>|0x01f|+0x01a:'45'
		// ******
	}

	// STATE[STUB]
	virtual	void	execute					( )
	{
		// LOCALS
		// network_core::packet_reader 		reader
		// ******

		// FUNCTION BODY[0xeb160]: 5
		// <0xeb171>|0x011|+0x020:'50'
		// <0xeb191>|0x031|+0x015:'51'
		// <0>
		// <0xeb1a6>|0x046|+0x026:'53'
		// <0xeb1cc>|0x06c|+0x01c:'54'
		// ******
	}

private:
	network_core::udp_match_stats		m_copied_stats;
	boost::function< void ( network_core::packet_reader& ) >	m_receiver;
	network_core::udp_match_packets_allocator_ptr	m_allocator;
	network_core::udp_match_packet&		m_packet;
	network_core::udp_match_stats&		m_target_stats;
}; // class receive_udp_response

STATIC_SIZE_ASSERT(receive_udp_response, 0xB8);

} // namespace network
} // namespace vostok

#endif // #ifndef RECEIVE_UDP_RESPONSE_H_INCLUDED
