////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_logic_base_state.h>

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

// STATE[STUB]
// static unsigned int survarium::player_logic_base_state::movement_animation_index(survarium::player_input const&)
u32 player_logic_base_state::movement_animation_index( player_input const& input )
{
	// LOCALS
	// bool 						move_bwd_pressed
	// bool 						move_right_pressed
	// bool 						move_fwd_pressed
	// bool 						move_left_pressed
	// ******

	return 0;

	// FUNCTION BODY
	// <0x59c104>|0x004|+0x012:'34'
	// <0x59c116>|0x016|+0x012:'35'
	// <0x59c128>|0x028|+0x012:'36'
	// <0x59c13a>|0x03a|+0x012:'37'
	// <0>
	// <0x59c14c>|0x04c|+0x010:'39'
	// <0x59c15c>|0x05c|+0x004:'40'
	// <0x59c160>|0x060|+0x004:'41'
	// <0>
	// <1>
	// <0x59c164>|0x064|+0x010:'44'
	// <0x59c174>|0x074|+0x004:'45'
	// <0x59c178>|0x078|+0x004:'46'
	// <0>
	// <1>
	// <0x59c17c>|0x07c|+0x008:'49'
	// <0x59c184>|0x084|+0x008:'50'
	// <0x59c18c>|0x08c|+0x007:'51'
	// <0>
	// <0x59c193>|0x093|+0x008:'53'
	// <0x59c19b>|0x09b|+0x007:'54'
	// <0>
	// <0x59c1a2>|0x0a2|+0x007:'56'
	// <0>
	// <1>
	// <0x59c1a9>|0x0a9|+0x008:'59'
	// <0x59c1b1>|0x0b1|+0x008:'60'
	// <0x59c1b9>|0x0b9|+0x007:'61'
	// <0>
	// <0x59c1c0>|0x0c0|+0x008:'63'
	// <0x59c1c8>|0x0c8|+0x007:'64'
	// <0>
	// <0x59c1cf>|0x0cf|+0x007:'66'
	// <0>
	// <1>
	// <0x59c1d6>|0x0d6|+0x008:'69'
	// <0x59c1de>|0x0de|+0x007:'70'
	// <0>
	// <0x59c1e5>|0x0e5|+0x008:'72'
	// <0x59c1ed>|0x0ed|+0x007:'73'
	// <0>
	// <0x59c1f4>|0x0f4|+0x002:'75'
	// ******
}

} // namespace survarium
