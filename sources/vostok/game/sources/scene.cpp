////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "scene.h"
#include "game.h"
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>
#include <vostok/input/gamepad.h>

namespace survarium {

scene::scene( game& g )
:m_game( g )
{
}

void scene::on_activate( )
{
	m_game.input_world().add_handler( *this );
}

void scene::on_deactivate( )
{
	m_game.input_world().remove_handler( *this );
}

bool scene::on_keyboard_action( vostok::input::world& input_world, vostok::input::enum_keyboard key, vostok::input::enum_keyboard_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS( &input_world, key, action );
	return false;
}

bool scene::on_gamepad_action( vostok::input::world& input_world, vostok::input::gamepad_button button, vostok::input::enum_gamepad_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS( &input_world, button, action );
	return false;
}

bool scene::on_mouse_key_action( vostok::input::world& input_world, vostok::input::mouse_button button, vostok::input::enum_mouse_key_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS( &input_world, button, action );
	return false;
}

bool scene::on_mouse_move( vostok::input::world& input_world, int x, int y, int z )
{
	VOSTOK_UNREFERENCED_PARAMETERS( &input_world, x, y, z );
	return false;
}

} // namespace survarium
