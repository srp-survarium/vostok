// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/game_core/player_input.h>

namespace survarium {

player_logic_base_state::player_logic_base_state( weapon_user_animations_selector& owner, const weapon_user_state_enum weapon_user_state_id ) :
	m_owner							( owner ),
	m_user							( NULL ),
	m_weapon_user_state_id			( weapon_user_state_id ),
	m_is_weapon_weapon_visible		( true ),
	m_is_smoothing_needed			( true ),
	m_is_physics_transform_allowed	( true ),
	m_is_ready_to_be_deactivated	( true )
{
}

void player_logic_base_state::set_user( base_player& user )
{
	m_user	= &user;
}

u32 player_logic_base_state::movement_animation_index( player_input const& input )
{
	bool	move_fwd_pressed	= ( input.actions_mask & 0x1 ) != 0;
	bool	move_bwd_pressed	= ( input.actions_mask & 0x2 ) != 0;
	bool	move_left_pressed	= ( input.actions_mask & 0x4 ) != 0;
	bool	move_right_pressed	= ( input.actions_mask & 0x8 ) != 0;

	if ( move_fwd_pressed && move_bwd_pressed )
	{
		move_fwd_pressed	= false;
		move_bwd_pressed	= false;
	}

	if ( move_left_pressed && move_right_pressed )
	{
		move_left_pressed	= false;
		move_right_pressed	= false;
	}

	// claude@MATCH: flat early-returns (no `else`) - an if/else-if chain would emit an
	// extra join `jmp` per level; flat returns each jump once to the single epilogue.
	if ( move_fwd_pressed )
	{
		if ( move_left_pressed )
			return move_fwd_left_anim;
		if ( move_right_pressed )
			return move_fwd_right_anim;
		return move_fwd_anim;
	}
	if ( move_bwd_pressed )
	{
		if ( move_left_pressed )
			return move_bwd_left_anim;
		if ( move_right_pressed )
			return move_bwd_right_anim;
		return move_bwd_anim;
	}
	if ( move_left_pressed )
		return move_left_anim;
	if ( move_right_pressed )
		return move_right_anim;
	return idle_anim;
}

} // namespace survarium
