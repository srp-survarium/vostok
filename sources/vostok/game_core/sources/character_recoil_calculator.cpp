// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/character_recoil_calculator.h>


#include <vostok/game_core/character_recoil_params.h>

namespace survarium {

character_recoil_calculator::character_recoil_calculator( ) :
	m_params		( NULL ),
	m_target_value	( 0.0f ),
	m_current_value	( 0.0f ),
	m_increase_speed( 1.0f ),
	m_decrease_speed( 1.0f ),
	m_current_time	( 0 )
{
}

void character_recoil_calculator::set_character_recoil_params( character_recoil_params const* params )
{
	m_params = params;
}

void character_recoil_calculator::tick(
	const weapon_user_state_enum	character_state,
	const bool						is_aiming,
	const u32						current_time_in_ms,
	const float						time_scale
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
	switch ( character_state )
	{
		case type_stand:
		case type_sprint:
		case type_jump:
		case type_preview:
			m_target_value = is_aiming ?  m_params->aimed_stand_multiplier : m_params->stand_multiplier;
		break;
		case type_crouch:
			m_target_value = is_aiming ?  m_params->aimed_crouch_multiplier : m_params->crouch_multiplier;
		break;
	}

	float dt = current_time_in_ms > m_current_time
		? ( current_time_in_ms - m_current_time ) * 0.001f * time_scale // sushi@TODO: What is the point of time_scale
		: 0.0f;

	m_current_time = current_time_in_ms;

	if ( m_current_value != m_target_value )
	{
		if ( m_target_value < m_current_value )
			m_current_value = math::max( m_current_value - ( m_decrease_speed * dt ), m_target_value );
		else
			m_current_value = math::min( m_current_value + ( m_increase_speed * dt ), m_target_value );
	}
}

} // namespace survarium
