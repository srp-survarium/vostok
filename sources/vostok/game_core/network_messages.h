////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_MESSAGES_H_INCLUDED
#define NETWORK_MESSAGES_H_INCLUDED

#include <vostok/network_core/udp_match_types.h>

namespace vostok {

// no PDB enum records (attested only as network_packets_orderer template args
// and match_client method params) - enumerators unknown, a matcher recovers
// them from the consuming asm; see the match-server first-byte note in
// <vostok/login_server/message_types.h> (cmp 0x80 in udp match receive)
enum match_client_message_types_enum
{
};
enum match_server_message_types_enum
{
};

} // namespace vostok

namespace survarium {

// canonical home: this header (the addressed get_sending_message_info inline
// lands at network_messages.h:'35'). PDB prints only the monomorphised
// < match_client_message_types_enum, match_server_message_types_enum > form -
// reconstructed as the primary template; the overloaded get_message_type_info
// statics take one template arg each
template < typename SendingMessageType, typename ReceivedMessageType >
class network_packets_orderer : public network_core::udp_match_packets_orderer {
public:
	inline	network_packets_orderer( ) { /* no source */ }

	// STATE[STUB]
	virtual	network_core::udp_match_message_type_info	get_sending_message_info	( u8 message_type )
	{
		// FUNCTION BODY[0xa7db0]: 1
		// <0xa7db0>|0x000|+0x007:'35'
		// ******

		// buildability return
		return network_core::udp_match_message_type_info( false, false, 0 );
	}

	// STATE[STUB]
	virtual	network_core::udp_match_message_type_info	get_received_message_info	( u8 arg_0 )
	{
		// buildability return
		return network_core::udp_match_message_type_info( false, false, 0 );
	}

	static	inline	network_core::udp_match_message_type_info	unreliable			( ) { /* no source */ return network_core::udp_match_message_type_info( false, false, 0 ); }
	static	inline	network_core::udp_match_message_type_info	reliable			( ) { /* no source */ return network_core::udp_match_message_type_info( false, false, 0 ); }
	static	inline	network_core::udp_match_message_type_info	ordered_reliable	( const u8 arg_0 ) { /* no source */ return network_core::udp_match_message_type_info( false, false, 0 ); }
	static	inline	network_core::udp_match_message_type_info	get_message_type_info( SendingMessageType arg_0 ) { /* no source */ return network_core::udp_match_message_type_info( false, false, 0 ); }
	static	inline	network_core::udp_match_message_type_info	get_message_type_info( ReceivedMessageType arg_0 ) { /* no source */ return network_core::udp_match_message_type_info( false, false, 0 ); }
}; // class network_packets_orderer

// comma in the template-id breaks the assert macro - alias first (the
// statistics_item.h precedent)
typedef network_packets_orderer<
	vostok::match_client_message_types_enum,
	vostok::match_server_message_types_enum
> match_network_packets_orderer;

STATIC_SIZE_ASSERT(match_network_packets_orderer, 0x4);

} // namespace survarium

#endif // #ifndef NETWORK_MESSAGES_H_INCLUDED
