////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_show_state_base.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_show_state_base::weapon_core_show_state_base(survarium::weapon_core&, bool&)
weapon_core_show_state_base::weapon_core_show_state_base( weapon_core& weapon, bool& is_shown ) :
	weapon_core_animation_end_aware_state	( weapon, true ), // sushi@TODO
	m_is_shown								( is_shown )
{
	// FUNCTION BODY
	// <0x6fd3d5>|0x055|+0x00d:'22'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_show_state_base::initialize()
void weapon_core_show_state_base::initialize( )
{
	// FUNCTION BODY
	// <0x6fd3f9>|0x009|+0x008:'27'
	// <0x6fd401>|0x011|+0x00c:'28'
	// <0>
	// <0x6fd40d>|0x01d|+0x00e:'30'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_show_state_base::finalize()
void weapon_core_show_state_base::finalize( )
{
	// FUNCTION BODY
	// <0x6fd357>|0x007|+0x008:'35'
	// <0>
	// <0x6fd35f>|0x00f|+0x00e:'37'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_show_state_base::on_animation_end_impl(bool&)
void weapon_core_show_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	// FUNCTION BODY
	// <0x6fd329>|0x009|+0x00c:'42'
	// <0x6fd335>|0x015|+0x00c:'43'
	// <0x6fd341>|0x021|+0x006:'44'
	// ******
}

} // namespace survarium
