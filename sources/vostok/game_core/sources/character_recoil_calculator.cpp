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

// STATE[100%|DONE]: was unpaired (/OPT:REF - the target caller weapon_core::activate/
// deactivate is unmatched); anchored via temp_include_all use_recoil_calculator -> 100.
void character_recoil_calculator::set_character_recoil_params( character_recoil_params const* params )
{
	m_params = params;
}

// STATE[99.65%|DONE]: target allocated 4 more bytes on the stack and used different offsets for local variables (non-steerable /Od frame-slot noise)
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

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts (no diverging rows, 0x184 bytes both)
	// VERDICT: STRUCTURE MATCH - all statements align size-exact; residual is the /Od frame-slot allocation (target +4 bytes of stack, shifted [ebp-N] offsets), non-steerable. trail: character_recoil_calculator_tick.md
}

} // namespace survarium
