////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/character_dispersion_calculator.h>
#include <vostok/game_core/character_dispersion_params.h>

namespace survarium {

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

// STATE[99.67%|PARTIAL]: every instruction/member-offset/call/constant byte-identical to target; the ONLY diff is target `sub esp,1Ch` (this@[ebp-10h]) vs base `sub esp,18h` (this@[ebp-0Ch]) - one extra unused 4-byte /Od frame slot shifting all [ebp-N] by 4. Non-steerable frame-allocation noise (same class as breath_vibration_calculator::tick). See character_dispersion_calculator_tick.md
void character_dispersion_calculator::tick(
	weapon_user_state_enum		character_state,
	bool						is_moving,
	bool						is_aiming,
	u8							broken_hands_count,
	bool						using_double_handed_weapon,
	u32							current_time_in_ms
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

	// LOCALS
	// float 						dt
	// ******

	// FUNCTION BODY
	// <0x596099>|0x009|+0x00c:'50'
	// <0x5960a5>|0x015|+0x009:'51'
	// <0x5960ae>|0x01e|+0x009:'52'
	// <0x5960b7>|0x027|+0x005:'53'
	// <0>
	// <1>
	// <0x5960bc>|0x02c|+0x00b:'56'
	// <0x5960c7>|0x037|+0x005:'57'
	// <0>
	// <0x5960cc>|0x03c|+0x01f:'59'
	// <0x5960eb>|0x05b|+0x009:'60'
	// <0>
	// <1>
	// <0x5960f4>|0x064|+0x034:'63'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x596128>|0x098|+0x02e:'69'
	// <0>
	// <1>
	// <2>
	// <0x596156>|0x0c6|+0x011:'73'
	// <0>
	// <0x596167>|0x0d7|+0x030:'75'
	// <0>
	// <1>
	// <0x596197>|0x107|+0x011:'78'
	// <0>
	// <0x5961a8>|0x118|+0x02a:'80'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[95.74%|INPROGRESS]: OUR base emits an extra `cmp 3; ja default` bounds check the target lacks (target source covers all 5 enum values + `default: NODEFAULT()`); fix deferred to a faster machine. See character_dispersion_calculator_get_target_koef.md
float character_dispersion_calculator::get_target_koef( weapon_user_state_enum character_state, bool is_moving, bool is_aiming ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );

	// claude@TODO (faster machine): two structural fixes, then rebuild + re-diff:
	//   1) no `cmp 3; ja default` in target - add `case type_preview: return 1.0f;` and change
	//      `default:` to `NODEFAULT();` (__assume(0)) so MSVC emits the contiguous [0..4] table.
	//   2) carcass shows `+0x002`/jmp `}` at 0x5d (line 108) & 0xc0 (line 114) -> the stand/crouch
	//      blocks were brace-scoped; add `{ }` to match. (95.74% is the pre-fix score.)
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

		default:
			return 1.0f;
	}

	// FUNCTION BODY
	// <0x595ee9>|0x009|+0x00c:'89'	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
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
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x595ef5>|0x015|+0x010:'104'	switch ( character_state ) {   ; target: jmp [edx*4+table], NO cmp/ja bounds check
	// <0>
	// <0x595f05>|0x025|+0x008:'106'	case type_stand: if ( is_moving )
	// <0x595f0d>|0x02d|+0x030:'107'	  return is_aiming ? walk_aim : walk;
	// <0x595f3d>|0x05d|+0x002:'108'	  jmp .crouch (fall-through thunk)
	// <0x595f3f>|0x05f|+0x02c:'109'	  else return is_aiming ? idle_aim : idle;
	// <0>
	// <1>
	// <0x595f6b>|0x08b|+0x008:'112'	case type_crouch: if ( is_moving )
	// <0x595f73>|0x093|+0x02d:'113'	  return is_aiming ? crouch_walk_aim : crouch_walk;
	// <0x595fa0>|0x0c0|+0x002:'114'	  jmp .sprint (fall-through thunk)
	// <0x595fa2>|0x0c2|+0x02d:'115'	  else return is_aiming ? crouch_aim : crouch;
	// <0>
	// <1>
	// <0x595fcf>|0x0ef|+0x00a:'118'	case type_sprint: return run;
	// <0>
	// <0x595fd9>|0x0f9|+0x00a:'120'	case type_jump:   return jump;
	// <0>
	// <0x595fe3>|0x103|+0x004:'122'	case type_preview: return 1.0f; (default: NODEFAULT())
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[93.33%|PARTIAL]: switch body matches instruction-for-instruction (case 0/default -> fld1; cases 1/2 are ternaries on using_double_handed_weapon). Residual + full asm in character_dispersion_calculator_get_broken_hands_penalty.md
// claude@MATCH: `case 0: return 1.0f;` (NOT `break;`) - break folds case 0 into default, drops the target's explicit `cmp 0`, and scores None.
float character_dispersion_calculator::get_broken_hands_penalty( u8 broken_hands_count, bool using_double_handed_weapon ) const
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
	}

	return 1.0f;

	// FUNCTION BODY
	// <0x595e59>|0x009|+0x00c:'130'	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
	// <0x595e65>|0x015|+0x018:'131'	switch ( broken_hands_count ) {
	// <0>
	// <0x595e7d>|0x02d|+0x004:'133'	case 0: return 1.0f;
	// <0>
	// <0x595e81>|0x031|+0x02b:'135'	case 1: return double_handed ? injury_double : 1.0f;
	// <0>
	// <0x595eac>|0x05c|+0x02b:'137'	case 2: return double_handed ? injury_double : injury_one;
	// <0>
	// <1>
	// <2>
	// ******
}

} // namespace survarium
