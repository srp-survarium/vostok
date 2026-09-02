// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef NETWORK_CORE_PROCESS_PACKET_PREDICATE_H_INCLUDED
#define NETWORK_CORE_PROCESS_PACKET_PREDICATE_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/network_core/udp_match_client.h>

namespace vostok {
namespace network_core {

class packet_reader;

class process_packet_predicate : private boost::noncopyable {
public:
	inline	explicit	process_packet_predicate	( udp_match_client& client ) :
		m_client	( client )
	{
	}

	// claude@MATCH: the inlined body at both call_predicate sites is the boost::function
	// safe-bool guard (empty()-fold call + `empty ? 0 : &dummy::nonnull` and-mask) then
	// the function2::operator() call on m_client.m_on_packet_received (@0x538).
	inline	void		operator()					( const u8 message_type, packet_reader& reader ) const
	{
		if ( m_client.m_on_packet_received )
			m_client.m_on_packet_received	( message_type, reader );
	}

	// trivial dtor (single reference member); empty body is correct. Inlined into the
	// process_incoming_packet/call_predicate scope - no standalone target symbol.
	inline				~process_packet_predicate	( ) { /* no source */ }

private:
	/* 0x0000 */	udp_match_client&		m_client;
}; // class process_packet_predicate

STATIC_SIZE_ASSERT(process_packet_predicate, 0x4);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PROCESS_PACKET_PREDICATE_H_INCLUDED
