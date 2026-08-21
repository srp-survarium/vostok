////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_menu.h"
#include "game.h"
#include "swf_input_translator.h"

#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>
#include <vostok/math_functions.h>
#include <vostok/math_uint2.h>

namespace survarium {

bool login_menu::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	get_game( ).input_translator( ).process_keyboard( input_world, key, action, m_login_menu_ui->movie, get_game( ).game_time_ms( ) );
	return true;
}

bool login_menu::on_gamepad_action(
	input::world*					__formal,
	input::gamepad_button			__formal2, // PDB: __formal too (collision)
	input::enum_gamepad_action		__formal3  // PDB: __formal too (collision)
)
{
	return true;
}

bool login_menu::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	u32 mouse_btn = 0;
	switch ( button )
	{
		case input::mouse_button_left:		mouse_btn = 0; break;
		case input::mouse_button_right:		mouse_btn = 1; break;
		case input::mouse_button_middle:	mouse_btn = 2; break;
	}

	m_login_menu_ui->movie->HandleMouseBtn(
		action == input::ms_key_down ? flash_movie::mouse_btn_down : flash_movie::mouse_btn_up,
		mouse_btn,
		( float )m_mouse_pos.x,
		( float )m_mouse_pos.y );

	return true;
}

bool login_menu::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	m_mouse_pos.x += x;
	m_mouse_pos.y += y;

	math::uint2 const& window_size = output_window_size( );
	math::clamp( m_mouse_pos.x, 0, ( s32 )window_size.x );
	math::clamp( m_mouse_pos.y, 0, ( s32 )window_size.y );

	m_login_menu_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, ( float )z );
	m_cursor_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, ( float )z );

	return true;
}

} // namespace survarium
