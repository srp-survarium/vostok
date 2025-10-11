////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_base_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_base_state::player_logic_base_state(survarium::weapon_user_animations_selector&, const survarium::weapon_user_state_enum)
player_logic_base_state::player_logic_base_state( weapon_user_animations_selector& owner, weapon_user_state_enum weapon_user_state_id )
{
}

// STATE[STUB]
// void survarium::player_logic_base_state::set_user(survarium::base_player&)
void player_logic_base_state::set_user( base_player& user )
{
	// FUNCTION BODY
	// <0x59c207>|0x000|0x000:'29'
	// ******
}

// STATE[STUB]
// static unsigned int survarium::player_logic_base_state::movement_animation_index(survarium::player_input const&)
static u32 player_logic_base_state::movement_animation_index( player_input const& input )
{
	// LOCALS
	// bool 						move_bwd_pressed
	// bool 						move_right_pressed
	// bool 						move_fwd_pressed
	// bool 						move_left_pressed
	// ******

	return 0;
	// FUNCTION BODY
	// <0x59c104>|0x000|0x000:'34'
	// <0x59c116>|0x012|0x012:'35'
	// <0x59c128>|0x024|0x012:'36'
	// <0x59c13a>|0x036|0x012:'37'
	// 1
	// <0x59c14c>|0x048|0x012:'39'
	// <0x59c15c>|0x058|0x010:'40'
	// <0x59c160>|0x05c|0x004:'41'
	// 1
	// 2
	// <0x59c164>|0x060|0x004:'44'
	// <0x59c174>|0x070|0x010:'45'
	// <0x59c178>|0x074|0x004:'46'
	// 1
	// 2
	// <0x59c17c>|0x078|0x004:'49'
	// <0x59c184>|0x080|0x008:'50'
	// <0x59c18c>|0x088|0x008:'51'
	// 1
	// <0x59c193>|0x08f|0x007:'53'
	// <0x59c19b>|0x097|0x008:'54'
	// 1
	// <0x59c1a2>|0x09e|0x007:'56'
	// 1
	// 2
	// <0x59c1a9>|0x0a5|0x007:'59'
	// <0x59c1b1>|0x0ad|0x008:'60'
	// <0x59c1b9>|0x0b5|0x008:'61'
	// 1
	// <0x59c1c0>|0x0bc|0x007:'63'
	// <0x59c1c8>|0x0c4|0x008:'64'
	// 1
	// <0x59c1cf>|0x0cb|0x007:'66'
	// 1
	// 2
	// <0x59c1d6>|0x0d2|0x007:'69'
	// <0x59c1de>|0x0da|0x008:'70'
	// 1
	// <0x59c1e5>|0x0e1|0x007:'72'
	// <0x59c1ed>|0x0e9|0x008:'73'
	// 1
	// <0x59c1f4>|0x0f0|0x007:'75'
	// ******
}

} // namespace survarium
