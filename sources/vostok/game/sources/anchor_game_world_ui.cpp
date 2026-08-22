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
#if 0	// pin-retirement probe: is game_world_ui reachable through the real call graph now?

	{ void ( game_world_ui::* const p )( match_options& )								= &game_world_ui::initialize;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( resources::unmanaged_resource_ptr const& )		= &game_world_ui::initialize_resources;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::on_unload;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( float3 )										= &game_world_ui::on_hit_from_pos;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_capture_progress;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_chat;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8, bool, bool )								= &game_world_ui::on_victory_item_put_take;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::on_enemy_hitted;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( u8, u8, bool, u32 )								= &game_world_ui::on_player_killed;				s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( pcstr )										= &game_world_ui::set_using_info_message;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::initialize_minimap;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::on_detached_from_player;		s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::hide_item_container;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( bool )											= &game_world_ui::show_players_list;			s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( const u32, const u32 )							= &game_world_ui::update_ui;					s_sink = *( pcvoid const* )&p; }
	{ void ( game_world_ui::* const p )( )												= &game_world_ui::update_minimap_objects;		s_sink = *( pcvoid const* )&p; }
	// update_quick_slot / create_slot_value are now private; they stay reachable through
	// the anchored public callers (update_ui -> update_quick_slot -> create_slot_value,
	// fill_quick_slots -> create_slot_value), so no direct member-pointer anchor is needed.
#endif
	( void )s_sink;
}

} // namespace survarium
