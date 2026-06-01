////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/game_core/player_input.h>

namespace survarium {

// STATE[STUB]
// survarium::player_logic_base_state::player_logic_base_state(survarium::weapon_user_animations_selector&, const survarium::weapon_user_state_enum)
player_logic_base_state::player_logic_base_state( weapon_user_animations_selector& owner, weapon_user_state_enum weapon_user_state_id ) :
	m_owner		( owner )
{
	// FUNCTION BODY
	// <0x59c220>|0x000|+0x052:'24'	{
	// <0x59c272>|0x052|      :'25'	}
	// ******
}

// STATE[STUB]
// void survarium::player_logic_base_state::set_user(survarium::base_player&)
void player_logic_base_state::set_user( base_player& user )
{
	// FUNCTION BODY
	// <0x59c207>|0x007|+0x009:'29'
	// ******
}

// STATE[100%|DONE]: byte-identical (91/91); flat early-returns match the single-jump epilogue
// static unsigned int survarium::player_logic_base_state::movement_animation_index(survarium::player_input const&)
u32 player_logic_base_state::movement_animation_index( player_input const& input )
{
	// claude@NOTE: bit masks read straight from the target asm (actions_mask @ +0x10);
	// the `!= 0` form matches is_sprinting()'s style and emits the and/neg/sbb/neg
	// boolean-normalize. Slot order [ebp-2],[ebp-4],[ebp-1],[ebp-3] is /Od slot
	// packing, not declaration order - declared fwd,bwd,left,right per carcass lines 34-37.
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

	// claude@MATCH: flat early-returns (no `else`) - each leaf jumps once to the single
	// epilogue, matching the target. An if/else-if/else chain emits an extra join `jmp`
	// per nesting level (the double-jmp diff).
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
