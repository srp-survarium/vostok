////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_dispersion_calculator.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_dispersion_calculator::weapon_dispersion_calculator()
weapon_dispersion_calculator::weapon_dispersion_calculator( )
{
	// FUNCTION BODY
	// <0x58f880>|0x000|+0x080:'21'	{
	// <0>
	// <0x58f900>|0x080|      :'23'	}
	// ******
}

// STATE[STUB]
// void survarium::weapon_dispersion_calculator::tick(const unsigned int)
void weapon_dispersion_calculator::tick( u32 current_time_in_ms )
{
	// LOCALS
	// float 						dt
	// ******

	// FUNCTION BODY
	// <0>
	// <0x58f999>|0x009|+0x009:'28'
	// <0>
	// <0x58f9a2>|0x012|+0x009:'30'
	// <0x58f9ab>|0x01b|+0x005:'31'
	// <0>
	// <0x58f9b0>|0x020|+0x00b:'33'
	// <0x58f9bb>|0x02b|+0x005:'34'
	// <0x58f9c0>|0x030|+0x01f:'35'
	// <0>
	// <0x58f9df>|0x04f|+0x009:'37'
	// <0x58f9e8>|0x058|+0x032:'38'
	// <0>
	// <1>
	// <0x58fa1a>|0x08a|+0x027:'41'
	// <0x58fa41>|0x0b1|+0x004:'42'
	// <0x58fa45>|0x0b5|+0x011:'43'
	// <0>
	// <0x58fa56>|0x0c6|+0x030:'45'
	// <0>
	// <1>
	// <0x58fa86>|0x0f6|+0x011:'48'
	// <0>
	// <0x58fa97>|0x107|+0x02a:'50'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
// void survarium::weapon_dispersion_calculator::fire()
void weapon_dispersion_calculator::fire( )
{
	// FUNCTION BODY
	// <0>
	// <0x58f959>|0x009|+0x024:'58'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_dispersion_calculator::reload()
void weapon_dispersion_calculator::reload( )
{
	// FUNCTION BODY
	// <0>
	// <0x58f919>|0x009|+0x025:'65'
	// <0>
	// ******
}

void weapon_dispersion_calculator::set_reload_dispersion_amount( const float reload_dispersion_amount )
{
	m_reload_dispersion_amount = reload_dispersion_amount;
}

void weapon_dispersion_calculator::set_one_shoot_dispersion_amount( const float one_shoot_dispersion_amount )
{
	m_one_shoot_dispersion_amount = one_shoot_dispersion_amount;
}

float weapon_dispersion_calculator::get_value( ) const
{
	return m_current_coeff;
}

void weapon_dispersion_calculator::set_aiming_speed( const float aiming_speed )
{
	m_aiming_speed = aiming_speed;
}

} // namespace survarium
