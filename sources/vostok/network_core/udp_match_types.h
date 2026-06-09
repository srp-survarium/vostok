////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED

namespace vostok {
namespace network_core {

// referenced only by reference/pointer in the udp_match_* classes
class	udp_match_client_session;

// packet-class tag stored in the first header byte; fill_packet_header folds the
// low-level flag into the acknowledgement word when the class is the low-level one.
enum udp_match_packets_count_enum
{
	udp_match_regular_packet	= 0x0,
	udp_match_low_level_packet	= 0x1,
}; // enum udp_match_packets_count_enum

struct udp_match_message_type_info {
	inline	udp_match_message_type_info	( bool is_reliable, bool is_ordered, u8 channel_id ) { /* no source */ }

public:
	/* 0x0000 */	u8		channel_id	: 6;
	/* 0x0000 */	u8		is_reliable	: 1;
	/* 0x0000 */	u8		is_ordered	: 1;
}; // struct udp_match_message_type_info

STATIC_SIZE_ASSERT(udp_match_message_type_info, 0x1);

struct udp_match_packets_orderer {
	virtual	udp_match_message_type_info	get_sending_message_info	( u8 message_type ) = 0;
	virtual	udp_match_message_type_info	get_received_message_info	( u8 message_type ) = 0;

	inline								udp_match_packets_orderer	( ) { /* no source */ }
}; // struct udp_match_packets_orderer

STATIC_SIZE_ASSERT(udp_match_packets_orderer, 0x4);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED
