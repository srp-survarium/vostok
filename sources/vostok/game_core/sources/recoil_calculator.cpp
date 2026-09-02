// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/recoil_calculator.h>

namespace survarium {

recoil_calculator::recoil_calculator( ) : m_weapon( NULL )
{
}

float recoil_calculator::get_horizontal_coeff( ) const
{
	return m_weapon_calculator.get_horizontal_koef( );
}

float recoil_calculator::get_vertical_coeff( ) const
{
	return m_weapon_calculator.get_vertical_koef( );
}

float recoil_calculator::get_back_coeff( ) const
{
	return m_weapon_calculator.get_back_koef( );
}

void recoil_calculator::tick(
	const weapon_user_state_enum	character_state,
	const bool						is_aiming,
	const u32						current_time_in_ms,
	const float						time_scale
)
{
	m_character_calculator.tick( character_state, is_aiming, current_time_in_ms, time_scale );
	m_weapon_calculator.tick( current_time_in_ms, time_scale );
	m_weapon_calculator.set_character_multiplier( m_character_calculator.get_value( ) );
}

void recoil_calculator::set_weapon( weapon_core* weapon )
{
	m_weapon = weapon;
	m_weapon_calculator.set_weapon( weapon );
}

void recoil_calculator::reload( )
{
	m_weapon_calculator.reload( );
}

void recoil_calculator::chamber_a_round( )
{
	m_weapon_calculator.chamber_a_round( );
}

void recoil_calculator::fire( )
{
	m_weapon_calculator.fire( );
}

} // namespace survarium
