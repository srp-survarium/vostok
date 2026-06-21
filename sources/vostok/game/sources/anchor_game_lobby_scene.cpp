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
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor

namespace survarium {

void use_game_lobby_scene( )
{
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
