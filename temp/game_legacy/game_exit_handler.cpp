////////////////////////////////////////////////////////////////////////////
//	Created		: 03.02.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "game_exit_handler.h"
#include "game.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>

using survarium::exit_handler;

exit_handler::exit_handler	(  survarium::game& game )
:m_game( game )
{
}

exit_handler::~exit_handler	( )
{
}

bool exit_handler::on_keyboard_action( vostok::input::world* input_world, vostok::input::enum_keyboard key, vostok::input::enum_keyboard_action action )
{
	if(action!=vostok::input::kb_key_down)
		return false;

	switch (key)
	{
		case vostok::input::key_f4:
		{
			if( input_world->get_keyboard()->is_key_down( vostok::input::key_lalt ) || 
				input_world->get_keyboard()->is_key_down( vostok::input::key_ralt )	)
			m_game.exit		( "quit" );
			return			true;
		}

		case vostok::input::key_grave:
		{
			m_game.toggle_console	( );
			return					true;
		}

		case vostok::input::key_escape:
		{
			m_game.scene_close_query( );
			return					true;
		}

		case vostok::input::key_f1:
		{
			m_game.switch_to_lobby( );
			return					true;
		}
	}
	return		false;
}

bool exit_handler::on_gamepad_action( vostok::input::world* input_world, vostok::input::gamepad_button button, vostok::input::enum_gamepad_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS		(action, button, input_world);
	return false;
}

bool exit_handler::on_mouse_key_action	( vostok::input::world* input_world, vostok::input::mouse_button button, vostok::input::enum_mouse_key_action action )
{
	//if ( button == vostok::input::mouse_button_left && action == vostok::input::ms_key_down &&
	//   ( input_world->get_keyboard()->is_key_down( vostok::input::key_lalt ) || input_world->get_keyboard()->is_key_down( vostok::input::key_ralt ) ) )
	//	m_game.setup_movement_target	( );
	return false;
}

bool exit_handler::on_mouse_move		( vostok::input::world* input_world, int x, int y, int z )
{
	VOSTOK_UNREFERENCED_PARAMETERS		(input_world, x, y, z);
	return false;
}
