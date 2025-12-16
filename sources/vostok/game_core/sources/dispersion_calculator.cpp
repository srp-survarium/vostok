////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/dispersion_calculator.h>

namespace survarium {

/*
// STATE[STUB]
// void `dynamic atexit destructor for 's_dispersion_enabled_cc''()
void `dynamic atexit destructor for 's_dispersion_enabled_cc''( )
{
	// FUNCTION BODY
	// <0x7dafa0>|0x000|      :'14'	{
	// ******
}
*/

// STATE[STUB]
// survarium::dispersion_calculator::dispersion_calculator()
dispersion_calculator::dispersion_calculator( )
{
	// FUNCTION BODY
	// <0x596920>|0x000|+0x044:'22'	{
	// <0x596964>|0x044|      :'23'	}
	// ******
}

// STATE[STUB]
// float survarium::dispersion_calculator::get_dispersion() const
float dispersion_calculator::get_dispersion( ) const
{
	// LOCALS
	// weapon_dispersion_params const& weapon_params
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x596980>|0x010|+0x07b:'27'
	// <0x5969fb>|0x08b|+0x007:'28'
	// <0x596a02>|0x092|+0x00f:'29'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x596a11>|0x0a1|+0x0ef:'40'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::set_shooting_skill_coeff(const float)
void dispersion_calculator::set_shooting_skill_coeff( float shooting_skill_coef )
{
	// FUNCTION BODY
	// <0x596787>|0x007|+0x00d:'70'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::set_aiming_speed_coeff(const float)
void dispersion_calculator::set_aiming_speed_coeff( float aiming_speed_coeff )
{
	// FUNCTION BODY
	// <0x596817>|0x007|+0x00d:'75'
	// <0x596824>|0x014|+0x008:'76'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::apply_aim_speed()
void dispersion_calculator::apply_aim_speed( )
{
	// LOCALS
	// float 						speed_of_aiming
	// ******

	// FUNCTION BODY
	// <0x5967a9>|0x009|+0x03d:'81'
	// <0x5967e6>|0x046|+0x00d:'82'
	// <0x5967f3>|0x053|+0x00f:'83'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::set_weapon(survarium::weapon_core*)
void dispersion_calculator::set_weapon( weapon_core* weapon )
{
	// FUNCTION BODY
	// <0x5968c7>|0x007|+0x009:'88'
	// <0x5968d0>|0x010|+0x009:'89'
	// <0>
	// <0x5968d9>|0x019|+0x018:'91'
	// <0x5968f1>|0x031|+0x018:'92'
	// <0>
	// <0x596909>|0x049|+0x008:'94'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::tick(const survarium::weapon_user_state_enum, const bool, const bool, const unsigned char, const bool, const unsigned int)
void dispersion_calculator::tick(
	weapon_user_state_enum		character_state,
	bool						is_moving,
	bool						is_aiming,
	u8							broken_hands_count,
	bool						using_double_handed_weapon,
	u32							current_time_in_ms
)
{
	// FUNCTION BODY
	// <0x596887>|0x007|+0x00c:'100'
	// <0x596893>|0x013|+0x027:'101'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::reload()
void dispersion_calculator::reload( )
{
	// FUNCTION BODY
	// <0x596867>|0x007|+0x008:'106'
	// ******
}

// STATE[STUB]
// void survarium::dispersion_calculator::fire()
void dispersion_calculator::fire( )
{
	// FUNCTION BODY
	// <0x596847>|0x007|+0x008:'111'
	// ******
}

} // namespace survarium
