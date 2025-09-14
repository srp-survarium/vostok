////////////////////////////////////////////////////////////////////////////
//	Created		: 06.10.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "npc_manipulation_handler.h"
#include "game.h"
#include <vostok/input/keyboard.h>
#include <vostok/input/world.h>
#include <vostok/input/mouse.h>

namespace survarium {

npc_manipulation_handler::npc_manipulation_handler	( game& environment ) :
	m_game											( environment )
{
}

npc_manipulation_handler::~npc_manipulation_handler( )
{
}

static bool are_alt_shift_pressed					( vostok::input::world* input_world )
{
	return ( ( input_world->get_keyboard()->is_key_down( vostok::input::key_lalt ) || input_world->get_keyboard()->is_key_down( vostok::input::key_ralt ) ) &&
		   ( input_world->get_keyboard()->is_key_down( vostok::input::key_lshift ) || input_world->get_keyboard()->is_key_down( vostok::input::key_rshift ) ) );
}

static bool are_ctrl_shift_pressed					( vostok::input::world* input_world )
{
	return ( ( input_world->get_keyboard()->is_key_down( vostok::input::key_lcontrol ) || input_world->get_keyboard()->is_key_down( vostok::input::key_rcontrol ) ) &&
		   ( input_world->get_keyboard()->is_key_down( vostok::input::key_lshift ) || input_world->get_keyboard()->is_key_down( vostok::input::key_rshift ) ) );
}

bool npc_manipulation_handler::on_keyboard_action	( vostok::input::world* input_world, vostok::input::enum_keyboard key, vostok::input::enum_keyboard_action action )
{
	if ( action != vostok::input::kb_key_down )
		return										false;

	switch ( key )
	{
		case vostok::input::key_8:
		case vostok::input::key_multiply:
		{
			if ( are_alt_shift_pressed( input_world ) )
			{
				m_game.toggle_npc_creation_mode		( );
				return								true;
			}
			break;
		}

		case vostok::input::key_1:
		case vostok::input::key_numpad1:
		{
			if ( are_alt_shift_pressed( input_world ) )
			{
				m_game.query_npc					( human_npc_neutral, m_game.get_camera_position() );
				return								true;
			}
			break;
		}

		case vostok::input::key_2:
		case vostok::input::key_numpad2:
		{
			if ( are_alt_shift_pressed( input_world ) )
			{
				m_game.query_npc					( human_npc_aggressive, m_game.get_camera_position() );
				return								true;
			}
			break;
		}

		case vostok::input::key_right:
		case vostok::input::key_numpad6:
		{
			if ( are_alt_shift_pressed( input_world ) )
			{
				m_game.rotate_selected_npc			( -90.f );
				return								true;
			}
			if ( are_ctrl_shift_pressed( input_world ) )
			{
				m_game.rotate_selected_npc			( -45.f );
				return								true;
			}
			break;
		}

		case vostok::input::key_left:
		case vostok::input::key_numpad4:
		{
			if ( are_alt_shift_pressed( input_world ) )
			{
				m_game.rotate_selected_npc			( 90.f );
				return								true;
			}
			if ( are_ctrl_shift_pressed( input_world ) )
			{
				m_game.rotate_selected_npc			( 45.f );
				return								true;
			}
			break;
		}

		case vostok::input::key_delete:
			m_game.delete_selected_npc				( );
		break;

		case vostok::input::key_b:
		{
			if ( are_alt_shift_pressed( input_world ) )
			{
				m_game.assign_behaviour				( );
				return								true;
			}
			break;
		}
	}
	return											false;
}

bool npc_manipulation_handler::on_gamepad_action	(
		vostok::input::world* input_world,
		vostok::input::gamepad_button button,
		vostok::input::enum_gamepad_action action
	)
{
	VOSTOK_UNREFERENCED_PARAMETERS					( action, button, input_world );
	return											false;
}

bool npc_manipulation_handler::on_mouse_key_action	(
		vostok::input::world* input_world,
		vostok::input::mouse_button button,
		vostok::input::enum_mouse_key_action action
	)
{
	if ( button == vostok::input::mouse_button_left && action == vostok::input::ms_key_up &&
	   ( input_world->get_keyboard()->is_key_down( vostok::input::key_lalt ) || input_world->get_keyboard()->is_key_down( vostok::input::key_ralt ) ) )
		m_game.check_selected_npc					( );
	
	return											false;
}

bool npc_manipulation_handler::on_mouse_move		( vostok::input::world* input_world, int x, int y, int z )
{
	VOSTOK_UNREFERENCED_PARAMETERS					( input_world, x, y, z );
	return											false;
}

} // namespace survarium