// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_MOVE_TO_LIST_PREDICATE_H_INCLUDED
#define NETWORK_CORE_MOVE_TO_LIST_PREDICATE_H_INCLUDED

#include <vostok/network_core/udp_match_connection.h>
#include <boost/noncopyable.hpp>

namespace vostok {
namespace network_core {

class udp_match_packet;

class move_to_list_predicate : private boost::noncopyable {
public:
	inline			move_to_list_predicate	(
						udp_match_connection::udp_match_packet_list&	list_to_move_to,
						pcstr const						logging_id,
						const u32						current_time_in_ms,
						const u32						max_time_delta
					) :
		m_list_to_move_to	( list_to_move_to ),
		m_logging_id		( logging_id ),
		m_current_time_in_ms	( current_time_in_ms ),
		m_max_time_delta	( max_time_delta )
	{
	}

	inline	bool	operator()				( udp_match_packet* const packet ) const
	{
		if ( m_current_time_in_ms < packet->last_send_time_in_ms + m_max_time_delta )
			return false;

		m_list_to_move_to.push_back( packet );
		return true;
	}

	// trivial dtor (reference + POD members); empty body is correct. Inlined into the
	// remove_if scope in udp_match_connection.cpp - no standalone target symbol.
	inline			~move_to_list_predicate	( ) { /* no source */ }

private:
	/* 0x0000 */	udp_match_connection::udp_match_packet_list&	m_list_to_move_to;
	/* 0x0004 */	pcstr const					m_logging_id;
	/* 0x0008 */	const u32					m_current_time_in_ms;
	/* 0x000c */	const u32					m_max_time_delta;
}; // class move_to_list_predicate

STATIC_SIZE_ASSERT(move_to_list_predicate, 0x10);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_MOVE_TO_LIST_PREDICATE_H_INCLUDED
