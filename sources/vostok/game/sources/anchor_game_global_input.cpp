////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for survarium::global_input_handler
// (global_input_handler.cpp). Its sole constructor is the function-local
// `static global_input_handler g_input_handler` initializer inside
// game::on_base_resources_created, which is still a STUB - so nothing in the
// linked EXE references the ctor / dtor / virtual overrides and /OPT:REF strips
// them, leaving the delinker no base symbol to score.
//
// use_game_global_input() constructs a global_input_handler on a placeholder
// game& and calls every out-of-line method; the whole body is guarded by a
// never-true static volatile flag so it never runs. The calls are hard
// references LTCG can not elide, keeping the carcass objects in the base EXE.
//
// Retire once game::on_base_resources_created is bodied and the real call graph
// (the g_input_handler static init) reaches these.

#include "pch.h"

#include "global_input_handler.h"
#include "game.h"
#include <vostok/input/keyboard.h>
#include <vostok/input/gamepad.h>
#include <vostok/input/mouse.h>

namespace survarium {

void use_game_global_input( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	static game* volatile			s_game		= 0;
	static input::world* volatile	s_world		= 0;
	static s32 volatile				s_i			= 0;

	global_input_handler			handler( *s_game );
	input::world*					world	= s_world;
	const s32						i		= s_i;

	handler.on_keyboard_action( world, ( input::enum_keyboard )i, ( input::enum_keyboard_action )i );
	handler.on_gamepad_action( world, ( input::gamepad_button )i, ( input::enum_gamepad_action )i );
	handler.on_mouse_key_action( world, ( input::mouse_button )i, ( input::enum_mouse_key_action )i );
	handler.on_mouse_move( world, i, i, i );
	handler.input_priority( );
}

} // namespace survarium
