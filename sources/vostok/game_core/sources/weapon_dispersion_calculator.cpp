////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_dispersion_calculator.h>

namespace survarium {

// STATE[STUB]
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

// STATE[94.46%|INPROGRESS]: frame is 4 bytes short (sub esp,14h vs 18h); source
// logic and structure (14 stmts) match the target. The missing 4-byte stack slot
// shifts this at [ebp-0ch] vs [ebp-10h], cascading through every member access
// and flipping the && short-circuit from a direct near-jmp to a short-jmp relay
// (the TRGT_ONLY 4-byte dead-jmp pair). Next step: identify the source of the
// extra 4-byte stack allocation in the target.
void weapon_dispersion_calculator::tick( u32 current_time_in_ms )
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

// STATE[100%|DONE]
void weapon_dispersion_calculator::fire( )
{
	m_target_coeff = math::min( m_target_coeff + m_one_shoot_dispersion_amount, m_max_value );
}

// STATE[100%|DONE]
void weapon_dispersion_calculator::reload( )
{
	m_target_coeff = math::min( m_target_coeff + m_reload_dispersion_amount, m_max_value );
}

// STATE[100%|DONE]
void weapon_dispersion_calculator::set_reload_dispersion_amount( const float reload_dispersion_amount )
{
	m_reload_dispersion_amount = reload_dispersion_amount;
}

// STATE[100%|DONE]
void weapon_dispersion_calculator::set_one_shoot_dispersion_amount( const float one_shoot_dispersion_amount )
{
	m_one_shoot_dispersion_amount = one_shoot_dispersion_amount;
}

// STATE[100%|DONE]
float weapon_dispersion_calculator::get_value( ) const
{
	return m_current_coeff;
}

// STATE[100%|DONE]
void weapon_dispersion_calculator::set_aiming_speed( const float aiming_speed )
{
	m_aiming_speed = aiming_speed;
}

} // namespace survarium
