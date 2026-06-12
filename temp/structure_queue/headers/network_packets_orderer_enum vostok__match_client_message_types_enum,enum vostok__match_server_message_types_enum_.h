////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_PACKETS_ORDERER_H_INCLUDED
#define NETWORK_PACKETS_ORDERER_H_INCLUDED

/* INCLUDES */
struct vostok::network_core::udp_match_packets_orderer;
enum vostok::match_client_message_types_enum;
enum vostok::match_server_message_types_enum;
class vostok::network_core::udp_match_message_type_info;

namespace survarium {

class network_packets_orderer< enum match_client_message_types_enum, enum match_server_message_types_enum > : public network_core::udp_match_packets_orderer {
public:
	virtual	network_core::udp_match_message_type_info	get_sending_message_info	( u8 message_type ) override { /* no source */ }
	virtual	network_core::udp_match_message_type_info	get_received_message_info	( u8 arg_0 ) override { /* no source */ }

	inline		network_packets_orderer<enum vostok::match_client_message_types_enum,enum vostok::match_server_message_types_enum>( ) { /* no source */ }

	static	inline	network_core::udp_match_message_type_info	unreliable					( ) { /* no source */ }
	static	inline	network_core::udp_match_message_type_info	reliable					( ) { /* no source */ }
	static	inline	network_core::udp_match_message_type_info	ordered_reliable			( const u8 arg_0 ) { /* no source */ }
	static	inline	network_core::udp_match_message_type_info	get_message_type_info		( match_client_message_types_enum arg_0 ) { /* no source */ }
	static	inline	network_core::udp_match_message_type_info	get_message_type_info		( match_server_message_types_enum arg_0 ) { /* no source */ }
}; // class network_packets_orderer< enum match_client_message_types_enum, enum match_server_message_types_enum >

STATIC_SIZE_ASSERT(network_packets_orderer< enum match_client_message_types_enum, enum match_server_message_types_enum >, 0x4);

} // namespace survarium

#endif // #ifndef NETWORK_PACKETS_ORDERER_H_INCLUDED
