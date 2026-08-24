////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED

namespace vostok {
namespace network_core {

// referenced only by reference/pointer in the udp_match_* classes
class	udp_match_client_session;

// packet-count tag stored in the first header byte; fill_packet_header folds the
// multiple-packets flag into the acknowledgement word when the class is the multi one.
// canonical: headers/vostok/network_core/enums/udp_match_packets_count_enum.h
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

	// STATE[REMOVED]: abstract base (pure virtuals) - never directly constructed; the
	// concrete derived orderer (its ctor folds this in) lives outside network_core, and
	// in scope the orderer is only held by-reference. Uninstantiated both sides.
	inline								udp_match_packets_orderer	( ) { /* no source */ } // STATE[REMOVED]
}; // struct udp_match_packets_orderer

STATIC_SIZE_ASSERT(udp_match_packets_orderer, 0x4);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED
