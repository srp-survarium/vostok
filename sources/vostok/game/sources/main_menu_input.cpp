// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "main_menu.h"

namespace survarium {

bool main_menu::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, key, action );
	return false;
}

bool main_menu::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, button, action );
	return false;
}

bool main_menu::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, button, action );
	return true;
}

bool main_menu::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, x, y, z );
	return true;
}

} // namespace survarium
