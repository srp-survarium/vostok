// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "game_world.h"

#include <vostok/input/keyboard.h>
#include <vostok/input/world.h>

#include "game.h"
#include "chat_handler.h"
#include "key_binder.h"
#include "base_network_client.h"

namespace survarium {

// claude@NOTE: structure matches the target, but the residual % is callee-stub
// inlining - show_players_list, key_binder::get_binded_action,
// chat_handler::focus and game::activate_main_menu are still STUBs, so under
// LTCG their calls fold away (get_binded_action returns a constant, collapsing
// the game_action branches). Unblocks once those callee bodies are matched.
bool game_world::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	bool btab = input_world->get_keyboard()->is_key_down( input::key_tab );
	game_ui.show_players_list( btab );

	if ( action == input::kb_key_down )
	{
		toggle_action_enum	action_type;
		game_action_id		game_action	=
			get_game().get_key_binder().get_binded_action( key, action_type, 1 );

		if ( game_action == kCHAT || key == input::key_return || key == input::key_numpadenter )
			get_game().get_chat_handler().focus( true );

		if ( key == input::key_k )
		{
			get_game().get_network_client()->initiate_kill_current_player();
			return true;
		}

		if ( key == input::key_l )
		{
			get_game().get_network_client()->initiate_respawn_current_player();
			return true;
		}

		if ( key == input::key_escape )
		{
			get_game().activate_main_menu();
			return true;
		}

		if ( game_action == kCAM_1 )
			switch_to_player_camera( true );
	}

	return false;
}

bool game_world::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return false;
}

bool game_world::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	return false;
}

bool game_world::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;
}


} // namespace survarium
