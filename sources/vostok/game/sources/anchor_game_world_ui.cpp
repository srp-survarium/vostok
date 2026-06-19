////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for game_world_ui. The class lives as
// a game_world member (game_world::game_ui) but the game_world ctor is a
// never-instantiated stub, so its public methods would be /OPT:REF-stripped from
// the base EXE. anchor_game_world_ui() address-takes every public method through a
// volatile sink so the linker keeps them for the delinker to score (a stub body
// references nothing on its own; here the bodies are matched, but nothing reachable
// calls them yet).
//
// Retire once the real game call graph (game_world / weapon / hud callers) reaches
// these for itself.

#include "pch.h"
#include "game_world_ui.h"

namespace survarium {

void anchor_game_world_ui( )
{
	static pcvoid volatile s_sink = 0;

	{ void ( game_world_ui::* const p )( match_options& )								= &game_world_ui::initialize;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( network_core::packet_reader& )					= &game_world_ui::initialize_base_points;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( resources::unmanaged_resource_ptr const& )		= &game_world_ui::initialize_resources;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u32, u32 )										= &game_world_ui::set_base_capture_progress;	s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u32 )											= &game_world_ui::set_match_time;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u32 )											= &game_world_ui::set_respawn_time;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr )										= &game_world_ui::set_broken_connection_message;s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_pregame;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr, u32 )									= &game_world_ui::set_pregame;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8, u32, u32 )									= &game_world_ui::set_player_kills_deaths;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::on_unload;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8 )											= &game_world_ui::set_health;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( float3 )										= &game_world_ui::on_hit_from_pos;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( float )										= &game_world_ui::set_crosshair_size;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_crosshair;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_ammo_indicator;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_capture_progress;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_chat;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_quick_slots;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::fill_quick_slots;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( profile_slot_enum )								= &game_world_ui::add_quick_slot_to_update;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( profile_slot_enum )								= &game_world_ui::disactivate_quick_slot;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8, bool, bool )								= &game_world_ui::on_victory_item_put_take;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::on_enemy_hitted;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8, u8, bool, u32 )								= &game_world_ui::on_player_killed;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( const u32 )									= &game_world_ui::set_fire_queue_size;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( const u32 )									= &game_world_ui::set_ammo_in_magazine;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( const u8 )										= &game_world_ui::set_ammo_type;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u32, u32 )										= &game_world_ui::set_ammo_total_count;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr )										= &game_world_ui::show_screen_message;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr, u8, u8, u32 )							= &game_world_ui::show_parametrized_message;	s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u32 )											= &game_world_ui::set_using_progress_message;	s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr )										= &game_world_ui::set_using_info_message;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::initialize_minimap;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::update_minimap_players;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::update_minimap_local_player;	s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::reset_map_rotatable;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( s8, s8 )										= &game_world_ui::add_victory_points;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( s8, s8 )										= &game_world_ui::set_victory_points;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( player_ptr )									= &game_world_ui::on_attached_to_player;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::on_detached_from_player;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u32, bool )									= &game_world_ui::set_player_online_status;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8 )											= &game_world_ui::show_item_container;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::hide_item_container;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr, const hit_affects_type_enum, const affect_event_type_enum )	= &game_world_ui::on_damage_affect_applying;	s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_players_list;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( const u32, const u32 )							= &game_world_ui::update_ui;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::update_minimap_objects;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( profile_slot_enum )								= &game_world_ui::update_quick_slot;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( profile_slot_enum, inventory_item_props&, flash_value& )	= &game_world_ui::create_slot_value;		s_sink = *( pcvoid const* )&p; }
}

} // namespace survarium
