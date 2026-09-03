// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ORDERER_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKETS_ORDERER_H_INCLUDED

namespace vostok {
namespace network_core {

class udp_match_client_session;

enum udp_match_packets_count_enum
{
	udp_match_single_packet		= 0x0,
	udp_match_multiple_packets	= 0x1,
}; // enum udp_match_packets_count_enum

struct udp_match_message_type_info {
	inline	udp_match_message_type_info	( const bool reliable, const bool ordered, const u8 channel ) :
		channel_id	( channel ),
		is_reliable	( reliable ),
		is_ordered	( ordered )
	{
	}

public:
	/* 0x0000 */	u8		channel_id	: 6;
	/* 0x0000 */	u8		is_reliable	: 1;
	/* 0x0000 */	u8		is_ordered	: 1;
}; // struct udp_match_message_type_info

STATIC_SIZE_ASSERT(udp_match_message_type_info, 0x1);

struct udp_match_packets_orderer {
	virtual	udp_match_message_type_info	get_sending_message_info	( u8 message_type ) = 0;
	virtual	udp_match_message_type_info	get_received_message_info	( u8 message_type ) = 0;

	inline								udp_match_packets_orderer	( ) { /* no source */ } // STATE[REMOVED]
}; // struct udp_match_packets_orderer

STATIC_SIZE_ASSERT(udp_match_packets_orderer, 0x4);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ORDERER_H_INCLUDED
