////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_menu.h"
#include "game.h"
#include "base_network_client.h"
#include "chat_handler.h"
#include "swf_input_translator.h"
#include "lobby_client.h"
#include "key_binder.h"

#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/input/world.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>
#include <vostok/math_functions.h>
#include <vostok/math_uint2.h>

namespace survarium {

lobby_client& lobby_menu::lobby_client( )
{
	return get_game( ).get_network_client( )->lobby_client( );
}

messaging_client& lobby_menu::messaging_client( )
{
	return get_game( ).get_network_client( )->messaging_client( );
}

bool lobby_menu::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	if ( m_is_connected_to_lobby && !m_is_in_match_making )
	{
		if ( action != input::kb_key_down )
		{
			get_game( ).input_translator( ).process_keyboard( input_world, key, action, m_lobby_menu_ui->movie, get_game( ).game_time_ms( ) );
			return false;
		}

		flash_value is_typing_text;
		m_lobby_menu_ui->movie->Invoke( "root.is_typing", &is_typing_text, NULL, 0 );

		if ( is_typing_text.GetBool( ) )
		{
			get_game( ).input_translator( ).process_keyboard( input_world, key, action, m_lobby_menu_ui->movie, get_game( ).game_time_ms( ) );
			return true;
		}

		toggle_action_enum action_type;
		switch ( get_game( ).get_key_binder( ).get_binded_action( key, action_type, 2 ) )
		{
			case kCHARACTER:	m_lobby_menu_ui->movie->Invoke( "root.show_character_toggle", NULL, NULL, 0 );	break;
			case kSHOP:			m_lobby_menu_ui->movie->Invoke( "root.show_shop_toggle", NULL, NULL, 0 );		break;
			case kINVENTORY:	m_lobby_menu_ui->movie->Invoke( "root.show_inventory_toggle", NULL, NULL, 0 );	break;
			case kFRIENDS:		m_lobby_menu_ui->movie->Invoke( "root.show_friends_toggle", NULL, NULL, 0 );	break;
		}

		if ( key == input::key_f5 )
			lobby_client( ).query_client_status( lobby::q_client_state );	// 0
		else if ( key == input::key_escape )
		{
			flash_value has_active_window;
			m_lobby_menu_ui->movie->Invoke( "root.has_active_window", &has_active_window, NULL, 0 );

			if ( has_active_window.GetBool( ) )
				m_lobby_menu_ui->movie->Invoke( "root.close_active_window", NULL, NULL, 0 );
			else
				get_game( ).activate_main_menu( );

			return true;
		}

		get_game( ).input_translator( ).process_keyboard( input_world, key, action, m_lobby_menu_ui->movie, get_game( ).game_time_ms( ) );
		return false;
	}

	return true;
}

bool lobby_menu::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( input_world, button, action );
	return false;
}

bool lobby_menu::on_mouse_key_action(
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

	get_game( ).get_chat_handler( ).get_movie( )->movie->HandleMouseBtn(
		( flash_movie::mouse_btn_action )action,
		mouse_btn,
		( float )m_mouse_pos.x,
		( float )m_mouse_pos.y );

	if ( m_is_connected_to_lobby )
	{
		flash_value v;
		m_message_ui->movie->Invoke( "root.anyMessage", &v, NULL, 0 );

		if ( v.GetBool( ) )
			input_translator( ).process_mouse_btn( input_world, button, action, ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, m_message_ui->movie );
		else
			input_translator( ).process_mouse_btn(
				input_world, button, action, ( float )m_mouse_pos.x, ( float )m_mouse_pos.y,
				( m_is_in_match_making ? m_match_making_ui : m_lobby_menu_ui )->movie );
	}

	return true;
}

bool lobby_menu::on_mouse_move(
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






	float scroll_delta = z * ( 1.f / 120.f ); m_cursor_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
	get_game( ).get_chat_handler( ).get_movie( )->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );

	if ( m_is_connected_to_lobby )
	{
		if ( m_is_in_match_making )
			m_match_making_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
		else
			m_lobby_menu_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
	}

	return true;
}

} // namespace survarium
