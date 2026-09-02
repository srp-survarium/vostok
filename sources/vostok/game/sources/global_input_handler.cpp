// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "global_input_handler.h"
#include "game.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>

namespace survarium {

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
