////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_MESSAGES_H_INCLUDED
#define NETWORK_MESSAGES_H_INCLUDED

#include <vostok/network_core/udp_match_types.h>

namespace vostok {

// canonical: headers/vostok/enums/match_client_message_types_enum.h
enum match_client_message_types_enum
{
	connection_request						= 0x40,
	get_startup_info						= 0x41,
	join_match								= 0x42,
	client_player_update					= 0x43,
	client_player_commit_suicide			= 0x44,
	time_synchronization_request			= 0x45,
	time_synchronization_confirmation		= 0x46,
	bullets_info_request					= 0x47,
	team_bases_initialize_info				= 0x48,
	force_finish_match						= 0x49,
	world_synchronization_confirmation		= 0x4a,
	match_client_invalid_message_type		= 0x7f,
};

// canonical: headers/vostok/enums/match_server_message_types_enum.h
enum match_server_message_types_enum
{
	match_server_connection_successful		= 0x80,
	match_options_message_type				= 0x81,
	server_player_input						= 0x82,
	kill_player								= 0x83,
	spawn_player							= 0x84,
	team_base_capture_progress				= 0x85,
	match_time_changed						= 0x86,
	respawn_time_changed					= 0x87,
	player_kd_stats_changed					= 0x88,
	hit_player								= 0x89,
	affect_damage_model						= 0x8a,
	sync_response							= 0x8b,
	match_finished							= 0x8c,
	server_bullet_added						= 0x8d,
	server_bullet_removed					= 0x8e,
	server_bullet_moved						= 0x8f,
	server_bullet_collided					= 0x90,
	player_visibility_changed				= 0x91,
	player_profile_message_type				= 0x92,
	team_bases_message_type					= 0x93,
	initialize_victory_items				= 0x94,
	victory_item_take_or_put				= 0x95,
	trap_placed								= 0x96,
	trap_removed							= 0x97,
	trap_fired								= 0x98,
	trap_disarmed							= 0x99,
	game_status_changed						= 0x9a,
	match_wait_time_changed					= 0x9b,
	game_world_object_state					= 0x9c,
	world_synchronization_request			= 0x9d,
	damage_model_state						= 0x9e,
	match_server_invalid_message_type		= 0xc0,
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
