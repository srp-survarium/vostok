////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the lobby character-preview scene
// cone (lobby_menu_scene.cpp's profile_player_character methods). The lobby_menu's
// own scene call graph is not yet reachable, so these out-of-line bodies get
// /OPT:REF-stripped from the base EXE and the delinker scores nothing. This anchor
// constructs a profile_player_character and references the matched methods so the
// linker keeps them. Self-guarded like the other game anchors so the placeholder
// lobby_menu& is never dereferenced.
//
// Only the methods whose bodies are reconstructed in lobby_menu_scene.cpp are
// referenced here; the query_resources/boost::bind-machinery methods remain
// /OPT:REF-stripped STUBs until matched.
//
// Retire once the real lobby-scene call graph reaches these for itself.

#include "pch.h"

#include "lobby_menu.h"
#include "profile_player_character.h"
#include "profile_character.h" // profile_character::character_animation_ready
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor

namespace survarium {

// pins lobby_menu_scene.cpp's file-local statics (the identity_transform_functor
// scene-load functor) - it cannot be reached cross-TU by address, so the TU anchors
// it itself and we just keep that anchor alive.
void pin_lobby_menu_scene_statics( );

void use_game_lobby_scene( )
{
	pin_lobby_menu_scene_statics( );

	// keep matched profile_character methods alive (their real call graph - the
	// query_resources callbacks - is not reachable yet); address-take since the
	// ctor needs live items_dictionary / scene_renderer / scene refs.
	static void ( profile_character::* volatile s_anim_ready )( resources::queries_result& )
		= &profile_character::character_animation_ready;
	(void)s_anim_ready;
	static void ( profile_character::* volatile s_model_ready )( resources::queries_result& )
		= &profile_character::character_model_ready;
	(void)s_model_ready;
	static void ( profile_character::* volatile s_weapon_ready )( resources::queries_result& )
		= &profile_character::weapon_resources_ready;
	(void)s_weapon_ready;

	static volatile bool s_run = false;
	if( !s_run )
		return;

	// source the lobby_menu& through a volatile pointer so LTCG cannot prove the
	// ctor's ref arg is the constant null this anchor passes and fold the
	// m_lobby_menu = lobby_menu store into m_lobby_menu = 0.
	static lobby_menu* volatile s_menu = 0;
	lobby_menu& menu = *s_menu;

	profile_player_character character( menu );

	character.update			( 0 );
	character.clear_resources	( );
	character.profile_changed	( NULL );
}

} // namespace survarium
