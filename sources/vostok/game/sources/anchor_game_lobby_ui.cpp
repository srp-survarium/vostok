////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the lobby-menu UI surface
// (lobby_menu_ui.cpp + lobby_menu_external_handler::callback + relocate_item_func).
// use_game_skeleton() constructs a lobby_menu but only calls a handful of methods,
// so the show_*/fill_*/update_*/on_* UI bodies get /OPT:REF-stripped from the base
// EXE and the delinker scores nothing. This anchor references every out-of-line
// lobby_menu_ui symbol so the linker keeps them. Self-guards like the other game
// anchors so the placeholder game& is never dereferenced.
//
// Retire once the real game call graph (game::create_lobby_menu + the menu's own
// callers) reaches these for itself.

#include "pch.h"

#include "lobby_menu.h"
#include "lobby_menu_external_handler.h"
#include "game.h"
#include <vostok/resources_queries_result.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>

namespace survarium {

// relocate_item_func is TU-local to lobby_menu_ui.cpp (no header); forward-declare
// the anchor hook there and call it from here.
void use_game_relocate_item_func( game& g );

void use_game_lobby_ui( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	game& g = *( game* )NULL;

	lobby_menu menu( g );

	menu.set_cursor						( 0 );
	menu.set_fps_stats					( 0.f );
	menu.set_ping						( 0 );
	menu.is_mouse_over_ui				( );

	menu.on_profile_arrived				( 0 );
	menu.on_price_items_arrived			( 0 );
	menu.on_shop_ui_ready				( );
	menu.on_ui_destroy					( );
	menu.on_match_message_arrived		( NULL );
	menu.on_stats_message_arrived		( NULL, NULL, messaging::server_message_channel );
	menu.on_player_reputations_arrived	( );

	menu.update_level_loading_progress	( );
	menu.reset_account_money			( );
	menu.on_render_scenes_ready			( *( resources::queries_result* )NULL );
	menu.player_parameters_ready		( *( resources::queries_result* )NULL, NULL );
	menu.update_ui						( 0, 0 );

	menu.fill_profiles					( );
	menu.fill_skills_tree				( );
	menu.on_items_compatibility_arrived	( );
	menu.on_slot_restrictions_arrived	( );
	menu.fill_character_data			( );
	menu.fill_inventory_contents		( );
	menu.on_profile_changed				( 0 );
	menu.fill_items_dictionary			( );
	menu.fill_inventory_labels			( );
	menu.fill_service_prices			( );
	menu.fill_friend_list				( );
	menu.fill_ignore_list				( );
	menu.fill_found_players				( );
	menu.show_match_making				( false );
	menu.update_status					( );
	menu.show_disconnected_message		( false );

	// lobby_menu_external_handler::callback (the flash external-interface dispatch)
	lobby_menu_external_handler handler( g );
	handler.callback					( NULL, NULL, NULL, 0 );

	use_game_relocate_item_func			( g );
}

} // namespace survarium
