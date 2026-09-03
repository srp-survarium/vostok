// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_MESSAGES_H_INCLUDED
#define NETWORK_MESSAGES_H_INCLUDED

#include <vostok/network/message_types.h>
#include <vostok/network_core/udp_match_packets_orderer.h>

namespace survarium {

// canonical home: this header (the addressed get_sending_message_info inline
// lands at network_messages.h:'35'). PDB prints only the monomorphised
// < match_client_message_types_enum, match_server_message_types_enum > form -
// reconstructed as the primary template; the overloaded get_message_type_info
// statics take one template arg each
template < typename SendingMessageType, typename ReceivedMessageType >
class network_packets_orderer : public network_core::udp_match_packets_orderer {
private:
	static	inline	network_core::udp_match_message_type_info	unreliable			( ) { return network_core::udp_match_message_type_info( false, false, 0 ); }
	static	inline	network_core::udp_match_message_type_info	reliable			( ) { return network_core::udp_match_message_type_info( true, false, 0 ); }
	static	inline	network_core::udp_match_message_type_info	ordered_reliable	( const u8 channel ) { return network_core::udp_match_message_type_info( true, true, channel ); }
	static	inline	network_core::udp_match_message_type_info	get_message_type_info( SendingMessageType ) { return ordered_reliable( 0 ); }
	static	inline	network_core::udp_match_message_type_info	get_message_type_info( ReceivedMessageType ) { return ordered_reliable( 0 ); }

	virtual	network_core::udp_match_message_type_info	get_sending_message_info	( u8 message_type )
	{
		return get_message_type_info( ( SendingMessageType )message_type );
	}

	virtual	network_core::udp_match_message_type_info	get_received_message_info	( u8 message_type )
	{
		return get_message_type_info( ( ReceivedMessageType )message_type );
	}
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
