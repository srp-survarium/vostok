// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_dispersion_calculator.h>

namespace survarium {

weapon_dispersion_calculator::weapon_dispersion_calculator( ) :
	m_one_shoot_dispersion_amount	( 0.0f ),
	m_reload_dispersion_amount		( 0.0f ),
	m_growth_speed					( 5.0f ),
	m_aiming_speed					( 0.0f ),
	m_max_value						( 1.0f ),
	m_target_coeff					( 0.0f ),
	m_current_coeff					( 0.0f ),
	m_current_time					( 0 )
{
}

void weapon_dispersion_calculator::tick( const u32 current_time_in_ms )
{
	if ( !m_current_time )
	{
		m_current_time = current_time_in_ms;
		return;
	}

	if ( m_current_time >= current_time_in_ms )
		return;

	const float dt = (float)( current_time_in_ms - m_current_time ) * 0.001f;

	m_current_time = current_time_in_ms;

	m_target_coeff = math::max( m_target_coeff - m_aiming_speed * dt, 0.0f );

	// claude@NOTE: target has an extra statement at line 42 - the outer if-body's
	//   scope exit emitted as two adjacent `jmp short .6` at label .3 (the && short-
	//   circuit-false skip and the inner else-if fall-through, /Od keeps them separate;
	//   14 stmts). Base /Od merges both into the if-condition's `jnp .4` (13 stmts).
	//   Byte content is identical; tried brace-less and Allman inner if/else-if (both
	//   byte-neutral, neither splits the jmp). /Od jump-fold quirk on the && + braced
	//   body, not steerable by brace placement here.
	if ( dt != 0.0f && m_current_coeff != m_target_coeff )
	{
		if ( m_current_coeff > m_target_coeff )
		{
			m_current_coeff = math::max( m_current_coeff - m_aiming_speed * dt, m_target_coeff );
		}
		else if ( m_current_coeff < m_target_coeff )
		{
			m_current_coeff = math::min( m_current_coeff + m_growth_speed * dt, m_target_coeff );
		}
	}
}

void weapon_dispersion_calculator::fire( )
{
	m_target_coeff = math::min( m_target_coeff + m_one_shoot_dispersion_amount, m_max_value );
}

void weapon_dispersion_calculator::reload( )
{
	m_target_coeff = math::min( m_target_coeff + m_reload_dispersion_amount, m_max_value );
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
