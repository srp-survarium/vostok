// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_WORLD_UI_H_INCLUDED
#define GAME_WORLD_UI_H_INCLUDED

#include <vostok/game_core/affect_event_type_enum.h>
#include <vostok/game_core/game_mode_type.h>
#include <vostok/game_core/hit_affects_type_enum.h>
#include <vostok/game_core/profile_slot_enum.h>

#include "base_point_stats.h"
#include <vostok/scaleform/sources/flash_movie_resource.h>

namespace vostok {
namespace network_core {
	class packet_reader;
} // namespace network_core
} // namespace vostok

namespace survarium {

class game_world;
class player;
struct flash_value;
struct inventory_item_props;
struct match_options;

// canonical UDT alias - player itself lives in player.h (batch 6), only the
// resource_ptr is passed by value here
typedef resources::resource_ptr<
	player,
	resources::unmanaged_intrusive_base
> player_ptr;

// the dump re-prints game_mode_type here; the real home is game_core (see
// game_mode_type.h) - included via the header list above, not redefined

class game_world_ui : private boost::noncopyable {
	typedef map< u32, base_point_stats, std::less< u32 > > base_points;

public:
			explicit					game_world_ui					( game_world& w );
	virtual								~game_world_ui					( );

			void						initialize						( match_options& options );

			void						initialize_base_points			( network_core::packet_reader& packet );
			void						initialize_resources			( resources::unmanaged_resource_ptr const& game_hud );

			void						set_base_capture_progress		( u32 progress, u32 point_id );
			void						set_match_time					( u32 time_left_ms );
			void						set_respawn_time				( u32 time_left );
			void						set_broken_connection_message	( pcstr str );

			void						show_pregame					( bool b_show );
			void						set_pregame						( pcstr str, u32 time_left );
			void						set_player_kills_deaths			( u8 player_id, u32 kills, u32 deaths );

			void						on_unload						( );

			void						set_health						( u8 health_in_percentage );

			void						on_hit_from_pos					( float3 position );

			void						set_crosshair_size				( float size );

			void						show_crosshair					( bool b_show );
			void						show_ammo_indicator				( bool b_show );
			void						show_capture_progress			( bool b_show );
			void						show_chat						( bool b_show );
			void						show_quick_slots				( bool b_show );
			void						fill_quick_slots				( );

			void						add_quick_slot_to_update		( profile_slot_enum slot );

			void						disactivate_quick_slot			( profile_slot_enum slot );

			void						on_victory_item_put_take		( u8 player_id, bool is_taken, bool is_base );
			void						on_enemy_hitted					( );
			void						on_player_killed				(
												u8		victim_id,
												u8		killer_id,
												bool	is_headshot,
												u32		item_dict_id
											);

			void						set_fire_queue_size				( const u32 fire_queue_size );
			void						set_ammo_in_magazine			( const u32 count );
			void						set_ammo_type					( const u8 ammo_type );
			void						set_ammo_total_count			( u32 first_type_count, u32 second_type_count );

			void						show_screen_message				( pcstr message_id );
			void						show_parametrized_message		(
												pcstr		message_id,
												u8			font_size,
												u8			y_pos_in_percents,
												u32			timeout_in_ms
											);
			void						set_using_progress_message		( u32 progress_value );
			void						set_using_info_message			( pcstr str );

			void						initialize_minimap				( );

			void						update_minimap_players			( );
			void						update_minimap_local_player		( );

			void						reset_map_rotatable				( );

			void						add_victory_points				( s8 team_1_points, s8 team_2_points );
			void						set_victory_points				( s8 team_1_points, s8 team_2_points );

			void						on_attached_to_player			( player_ptr player );
			void						on_detached_from_player			( );

			void						set_player_online_status		( u32 player_id, bool is_online );

			void						show_item_container				( u8 visual_id );
			void						hide_item_container				( );

			void						on_damage_affect_applying		(
												pcstr							bodypart,
												const hit_affects_type_enum		arg_1 /* hit_affects_type_enum affect */,
												const affect_event_type_enum	arg_2 /* affect_event_type_enum event_type */
											);

			void						show_players_list				( bool b_show );

			void						update_ui						( const u32 frame_delta_ms, const u32 __formal );
	inline	flash_movie_resource_ptr&	get_ui							( ) { /* no source */ return m_game_hud_ui; }

			void						update_minimap_objects			( );

private:
			void						update_quick_slot				( profile_slot_enum slot );

			void						create_slot_value				(
												profile_slot_enum			slot,
												inventory_item_props&		item_props,
												flash_value&				slot_descr_value
											);

private:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	flash_movie_resource_ptr			m_game_hud_ui;
	/* 0x0008 */	game_world&							m_game_world;
	/* 0x000c */	base_points							m_base_points;
	/* 0x0024 */	bool								m_players_list_visible;
	/* 0x0028 */	game_mode_type						m_game_mode;
	/* 0x002c */	u8									m_victory_items_count;
	/* 0x002e */	u16									m_match_time;
	/* 0x0030 */	u8									m_victory_points[2];
	/* 0x0034 */	vector< profile_slot_enum >			m_slots_to_update;
}; // class game_world_ui

STATIC_SIZE_ASSERT(game_world_ui, 0x40);

} // namespace survarium

#endif // #ifndef GAME_WORLD_UI_H_INCLUDED
