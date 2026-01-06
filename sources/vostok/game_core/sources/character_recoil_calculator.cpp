////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/character_recoil_calculator.h>


#include <vostok/game_core/character_recoil_params.h>

namespace survarium {

// STATE[100%|DONE]
character_recoil_calculator::character_recoil_calculator( ) :
	m_params		( NULL ),
	m_target_value	( 0.0f ),
	m_current_value	( 0.0f ),
	m_increase_speed( 1.0f ),
	m_decrease_speed( 1.0f ),
	m_current_time	( 0 )
{
}

// STATE[UNCHECKED]
void character_recoil_calculator::set_character_recoil_params( character_recoil_params const* params )
{
	m_params = params;

	// FUNCTION BODY
	// <0x5961e7>|0x007|+0x008:'27'
	// ******
}

// STATE[99.63%|DONE]: target allocated 4 more bytes on the stack and used different offsets for local variables
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

	// FUNCTION BODY
	// <0x596279>|0x009|+0x00c:'32'
	// <0x596285>|0x015|+0x016:'33'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x59629b>|0x02b|+0x035:'38'	m_target_value = is_aiming ?  m_params->aimed_stand_multiplier : m_params->stand_multiplier;
	// <0x5962d0>|0x060|+0x002:'39'
	// <0>
	// <0x5962d2>|0x062|+0x034:'41'	m_target_value = is_aiming ?  m_params->aimed_crouch_multiplier : m_params->crouch_multiplier;
	// <0>
	// <1>
	// <2>
	// <0x596306>|0x096|+0x046:'45'	float dt = current_time_in_ms > m_current_time
	// <0x59634c>|0x0dc|+0x009:'46'	m_current_time = current_time_in_ms;
	// <0x596355>|0x0e5|+0x015:'47'	if ( m_current_value != m_target_value )
	// <0>
	// <0x59636a>|0x0fa|+0x011:'49'		if ( m_target_value < m_current_value )
	// <0x59637b>|0x10b|+0x032:'50'
	// <0x5963ad>|0x13d|+0x002:'51'
	// <0x5963af>|0x13f|+0x02a:'52'
	// <0>
	// <1>
	// ******
}

} // namespace survarium
