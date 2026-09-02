// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RECEIVE_UDP_RESPONSE_H_INCLUDED
#define RECEIVE_UDP_RESPONSE_H_INCLUDED

#include "response.h"
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packets_allocator.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network {

class receive_udp_response :
	public response,
	private boost::noncopyable
{
public:
	// Target emits this constructor only inline in match_client::on_packet_received.
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

	// claude@NOTE: structure matches 2/2; residual is intrusive_ptr::operator*
	// (with its ASSERT eater) and the member ~intrusive_ptr (base inlines both,
	// target calls them out-of-line) - the inline-vs-call wall, not steerable.
	virtual			~receive_udp_response	( )
	{
		network_core::udp_match_packet* temp	= &m_packet;
		network_core::delete_udp_match_packet	( *m_allocator, temp );
	}

	// claude@NOTE: structure matches 4/4; residual is base_packet::buffer() inlined
	// inside the packet_reader-ctor expansion where the target calls the COMDAT
	// (same wall as receive_response::execute); the if/copy stats tail is byte-equal.
	virtual	void	execute					( )
	{
		network_core::packet_reader	reader( m_packet );
		m_receiver				( reader );

		if ( m_copied_stats >= m_target_stats )
			m_target_stats		= m_copied_stats;
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
