////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

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
	public boost::noncopyable
{
public:
	// STATE[INLINED]: no standalone symbol on either side; init list verified
	// against the inline expansion in match_client::on_packet_received (target rva
	// 0x74cdb0): copied_stats rep-movsd copy, receiver function1 copy-ctor,
	// allocator intrusive_ptr copy-ctor, packet address, target_stats reference
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

	// STATE[37.91%|PARTIAL]: statements correct; base INLINES
	// intrusive_ptr::operator* (with its ASSERT eater) and ~intrusive_ptr where
	// the target CALLS both out-of-line - whole-program LTCG inline-vs-call,
	// should flip as real consumers (match_client_impl) get matched
	virtual			~receive_udp_response	( )
	{
		network_core::udp_match_packet* temp	= &m_packet;
		network_core::delete_udp_match_packet	( *m_allocator, temp );
	}

	// STATE[88.51%|PARTIAL]: base inlines base_packet::buffer() inside the
	// packet_reader-ctor expansion where the target calls the COMDAT (same LTCG
	// wall as receive_response::execute); if/copy stats tail byte-equal
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
