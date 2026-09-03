// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_MESSAGE_TYPES_H_INCLUDED
#define NETWORK_MESSAGE_TYPES_H_INCLUDED

namespace vostok {

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

#endif // #ifndef NETWORK_MESSAGE_TYPES_H_INCLUDED
