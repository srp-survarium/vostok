// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/character_dispersion_calculator.h>
#include <vostok/game_core/character_dispersion_params.h>

namespace survarium {

character_dispersion_calculator::character_dispersion_calculator( )
	:	m_params					( NULL )
	,	m_target_value				( 0.0f )
	,	m_current_value				( 0.0f )
	,	m_value						( 0.0f )
	,	m_value_smoothing_speed		( 5.0f )
	,	m_aiming_speed				( 1.0f )
	,	m_current_time				( 0 )
	,	m_jumped					( false )
{
}

void character_dispersion_calculator::set_character_dispersion_params( character_dispersion_params const* character_params )
{
	m_params = character_params;
	if ( m_params )
	{
		m_target_value	= m_params->idle_multiplier;
		m_current_value	= m_params->idle_multiplier;
		m_value			= m_params->idle_multiplier;
	}
}

void character_dispersion_calculator::tick(
	const weapon_user_state_enum		character_state,
	const bool						is_moving,
	const bool						is_aiming,
	const u8							broken_hands_count,
	const bool						using_double_handed_weapon,
	const u32							current_time_in_ms
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );

	if ( m_current_time == 0 )
	{
		m_current_time = current_time_in_ms;
		return;
	}

	if ( m_current_time >= current_time_in_ms )
		return;

	float const dt = ( current_time_in_ms - m_current_time ) / 1000.0f;
	m_current_time = current_time_in_ms;

	m_target_value = get_target_koef( character_state, is_moving, is_aiming ) * get_broken_hands_penalty( broken_hands_count, using_double_handed_weapon );

	m_current_value = math::max( m_target_value, m_current_value - m_aiming_speed * dt );

	if ( m_value > m_current_value )
		m_value = math::max( m_current_value, m_value - m_value_smoothing_speed * dt );
	else if ( m_current_value > m_value )
		m_value = math::min( m_current_value, m_value + m_value_smoothing_speed * dt );
}

float character_dispersion_calculator::get_target_koef( const weapon_user_state_enum character_state, const bool is_moving, const bool is_aiming ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );

	switch ( character_state )
	{
		case type_stand:
			if ( is_moving )
				return is_aiming ? m_params->walk_aim_multiplier : m_params->walk_multiplier;
			else
				return is_aiming ? m_params->idle_aim_multiplier : m_params->idle_multiplier;

		case type_crouch:
			if ( is_moving )
				return is_aiming ? m_params->crouch_walk_aim_multiplier : m_params->crouch_walk_multiplier;
			else
				return is_aiming ? m_params->crouch_aim_multiplier : m_params->crouch_multiplier;

		case type_sprint:
			return m_params->run_multiplier;

		case type_jump:
			return m_params->jump_multiplier;

		case type_preview:
			return 1.0f;

		default:
			NODEFAULT();
	}
}

float character_dispersion_calculator::get_broken_hands_penalty( const u8 broken_hands_count, const bool using_double_handed_weapon ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );

	switch ( broken_hands_count )
	{
	case 0:
		return 1.0f;
	case 1:
		return using_double_handed_weapon ? m_params->injury_penalty_for_double_handed : 1.0f;
	case 2:
		return using_double_handed_weapon ? m_params->injury_penalty_for_double_handed : m_params->injury_penalty_for_one_handed;
	default:
		NODEFAULT();
	}
}

} // namespace survarium
