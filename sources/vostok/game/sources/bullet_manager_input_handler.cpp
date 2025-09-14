////////////////////////////////////////////////////////////////////////////
//	Created		: 21.07.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_manager_input_handler.h"
#include "bullet_manager.h"
#include "game.h"
#include "game_world.h"
//#include "game_camera.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>

namespace survarium{

bullet_manager_input_handler::bullet_manager_input_handler	( bullet_manager& bullet_manager )
:m_bullet_manager( bullet_manager )
{
	m_bullet_manager.get_game( ).input_world( ).add_handler( *this );
}

bullet_manager_input_handler::~bullet_manager_input_handler	( )
{
}

bool bullet_manager_input_handler::on_keyboard_action( vostok::input::world* input_world, vostok::input::enum_keyboard key, vostok::input::enum_keyboard_action action )
{
	VOSTOK_UNREFERENCED_PARAMETER		(input_world);

	if( action!=vostok::input::kb_key_down )
		return false;

	switch (key)
	{
		case vostok::input::key_f:
		{
			float4x4 view_transform = m_bullet_manager.get_game( ).get_game_world( ).get_camera_director( )->get_active_camera( )->get_inverted_view_matrix( );
			m_bullet_manager.fire( view_transform.c.xyz( ), view_transform.k.xyz( ) * 900 );
			return			true;
		}
		case vostok::input::key_g:
		{
#ifndef MASTER_GOLD

			m_bullet_manager.toggle_is_fixed( );
			return			true;

#endif // #ifndef MASTER_GOLD
		}
	}
	return		false;
}

bool bullet_manager_input_handler::on_gamepad_action	( vostok::input::world* input_world, vostok::input::gamepad_button button, vostok::input::enum_gamepad_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS		(action, button, input_world);
	return false;
}

bool bullet_manager_input_handler::on_mouse_key_action	( vostok::input::world* input_world, vostok::input::mouse_button button, vostok::input::enum_mouse_key_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS		(action, button, input_world);
	return false;
}

bool bullet_manager_input_handler::on_mouse_move		( vostok::input::world* input_world, int x, int y, int z )
{
	VOSTOK_UNREFERENCED_PARAMETERS		(input_world, x, y, z);
	return false;
}

} //namespace survarium