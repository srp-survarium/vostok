////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "global_input_handler.h"
#include "game.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>

namespace survarium {

// claude@NOTE: the target ctor is the constant-folded static-init for the single
// `static global_input_handler g_input_handler` inside game::on_base_resources_created
// (still a STUB): `this` folds to the global address, so it stores straight to
// [g_input_handler]/[g_input_handler+4] instead of via ecx. Built from the anchor's
// generic local it emits the register form (82.5%); reaches 100% once
// on_base_resources_created bodies the static and the real call graph reaches it.
global_input_handler::global_input_handler( game& game ) :
	m_game( game )
{
}

global_input_handler::~global_input_handler( )
{
}

bool global_input_handler::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	if ( action != input::kb_key_down )
		return false;

	switch ( key )
	{
		case input::key_f4:
			if ( input_world->get_keyboard()->is_key_down( input::key_lalt ) ||
				 input_world->get_keyboard()->is_key_down( input::key_ralt ) )
				m_game.exit( "quit" );
			else
				return false;
			break;

		case input::key_grave:
			m_game.toggle_console( );
			break;

		default:
			return false;
	}

	return true;
}

bool global_input_handler::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return false;
}

bool global_input_handler::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return false;
}

bool global_input_handler::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;
}


} // namespace survarium
