////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED

namespace vostok {
namespace network_core {

enum disconnect_event_types_enum
{
	disconnected_by_timeout				= 0x0,
	disconnected_by_connection_lost		= 0x1,
	disconnected_by_initiator			= 0x2,
}; // enum disconnect_event_types_enum

struct udp_match_message_type_info {
	inline		udp_match_message_type_info	( const bool reliable, const bool ordered, const u8 channel ) : channel_id( channel ), is_reliable( reliable ? 1 : 0 ), is_ordered( ordered ? 1 : 0 ) { }

public:
	/* 0x0000 */	u8		channel_id	: 6;
	/* 0x0000 */	u8		is_reliable	: 1;
	/* 0x0000 */	u8		is_ordered	: 1;
}; // struct udp_match_message_type_info

STATIC_SIZE_ASSERT( udp_match_message_type_info, 0x1 );

struct udp_match_packets_orderer {
	virtual	udp_match_message_type_info		get_sending_message_info	( u8 arg_0 ) = 0;
	virtual	udp_match_message_type_info		get_received_message_info	( u8 arg_0 ) = 0;

	inline									udp_match_packets_orderer	( ) { }
}; // struct udp_match_packets_orderer

STATIC_SIZE_ASSERT( udp_match_packets_orderer, 0x4 );

// referenced only by reference/pointer in the udp_match_* classes
class	udp_match_client_session;

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_TYPES_H_INCLUDED
