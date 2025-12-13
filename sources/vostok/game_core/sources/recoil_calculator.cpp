////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/recoil_calculator.h>

namespace survarium {

// STATE[STUB]
// survarium::recoil_calculator::recoil_calculator()
recoil_calculator::recoil_calculator( )
{
	// FUNCTION BODY
	// <0x593da0>|0x000|+0x024:'16'	{
	// <0x593dc4>|0x024|      :'17'	}
	// ******
}

// STATE[STUB]
// float survarium::recoil_calculator::get_horizontal_coeff() const
float recoil_calculator::get_horizontal_coeff( ) const
{
	return 0.0f;

	// FUNCTION BODY
	// <0x593d09>|0x009|+0x010:'21'
	// ******
}

// STATE[STUB]
// float survarium::recoil_calculator::get_vertical_coeff() const
float recoil_calculator::get_vertical_coeff( ) const
{
	return 0.0f;

	// FUNCTION BODY
	// <0x593ce9>|0x009|+0x010:'26'
	// ******
}

// STATE[STUB]
// float survarium::recoil_calculator::get_back_coeff() const
float recoil_calculator::get_back_coeff( ) const
{
	return 0.0f;

	// FUNCTION BODY
	// <0x593cc9>|0x009|+0x010:'31'
	// ******
}

// STATE[STUB]
// void survarium::recoil_calculator::tick(const survarium::weapon_user_state_enum, const bool, const unsigned int, const float)
void recoil_calculator::tick(
	weapon_user_state_enum		character_state,
	bool						is_aiming,
	u32							current_time_in_ms,
	float						time_scale
)
{
	// FUNCTION BODY
	// <0x593d49>|0x009|+0x01f:'36'
	// <0x593d68>|0x028|+0x013:'37'
	// <0x593d7b>|0x03b|+0x01a:'38'
	// ******
}

// STATE[STUB]
// void survarium::recoil_calculator::set_weapon(survarium::weapon_core*)
void recoil_calculator::set_weapon( weapon_core* weapon )
{
	// FUNCTION BODY
	// <0x593c57>|0x007|+0x009:'43'
	// <0x593c60>|0x010|+0x00c:'44'
	// ******
}

// STATE[STUB]
// void survarium::recoil_calculator::reload()
void recoil_calculator::reload( )
{
	// FUNCTION BODY
	// <0x593ca7>|0x007|+0x008:'49'
	// ******
}

// STATE[STUB]
// void survarium::recoil_calculator::chamber_a_round()
void recoil_calculator::chamber_a_round( )
{
	// FUNCTION BODY
	// <0x593c87>|0x007|+0x008:'54'
	// ******
}

// STATE[STUB]
// void survarium::recoil_calculator::fire()
void recoil_calculator::fire( )
{
	// FUNCTION BODY
	// <0x593d27>|0x007|+0x008:'59'
	// ******
}

} // namespace survarium
