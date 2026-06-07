////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/character_dispersion_calculator.h>
#include <vostok/game_core/character_dispersion_params.h>

namespace survarium {

// STATE[STUB]
// survarium::character_dispersion_calculator::character_dispersion_calculator()
character_dispersion_calculator::character_dispersion_calculator( )
{
	// FUNCTION BODY
	// <0x596010>|0x000|+0x079:'23'	{
	// <0x596089>|0x079|      :'24'	}
	// ******
}

// STATE[STUB]
// void survarium::character_dispersion_calculator::set_character_dispersion_params(survarium::character_dispersion_params const*)
void character_dispersion_calculator::set_character_dispersion_params( character_dispersion_params const* character_params )
{
	// FUNCTION BODY
	// <0x595e07>|0x007|+0x008:'28'
	// <0x595e0f>|0x00f|+0x008:'29'
	// <0x595e17>|0x017|+0x00d:'30'
	// <0x595e24>|0x024|+0x00d:'31'
	// <0x595e31>|0x031|+0x00d:'32'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::character_dispersion_calculator::tick(const survarium::weapon_user_state_enum, const bool, const bool, const unsigned char, const bool, const unsigned int)
void character_dispersion_calculator::tick(
	weapon_user_state_enum		character_state,
	bool						is_moving,
	bool						is_aiming,
	u8							broken_hands_count,
	bool						using_double_handed_weapon,
	u32							current_time_in_ms
)
{
	// claude@NOTE: STUB - only references the two private getters so the linker
	// keeps them alive (tick is the sole caller of get_target_koef /
	// get_broken_hands_penalty). Real tick body is not matched here.
	m_target_value = get_target_koef( character_state, is_moving, is_aiming )
		+ get_broken_hands_penalty( broken_hands_count, using_double_handed_weapon );

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

	// claude@TODO (faster machine): ONE structural fix, then rebuild + re-diff:
	//   no `cmp 3; ja default` in target - add `case type_preview: return 1.0f;` and change
	//   `default:` to `NODEFAULT();` (__assume(0)) so MSVC emits the contiguous [0..4] table.
	//   (structure-diff: 21/21 stmts, 0 quantity-diffs; both SIZE diffs are this one
	//   cause. The `+0x002` jmps at 0x5d/0xc0 are case fall-through thunks on BOTH
	//   sides, NOT a missing brace-block - no `{ }` change needed. 95.74% pre-fix.)
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

	// STRUCTURE DIFF:
	// target: 0x585ee0            base: 0x4543c0
	// ; float survarium::character_dispersion_calculator::get_target_koef(const survarium::weapon_user_state_enum, const bool, const bool) const ; target 19 stmts / base 19 stmts
	// .. same ..
	// 0x015 <0x10> | 0x015 <0x1a> | switch ( character_state )   SIZE
	// .. same ..
	// 0x103 <0x4> | 0x10d <0x2> | return 1.0f;   SIZE
	// .. same ..
	// ; aligned 17, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MISMATCH (size) - base switch emits a `cmp 3; ja default` bounds check target lacks; cover all enum values + `default: NODEFAULT();` to drop it  trail: character_dispersion-get_target_koef.md
}

// STATE[93.33%|INPROGRESS]: quantity divergence - base emits a trailing `return 1.0f;` (extra fld1 at 0x8b) that the target does NOT have. Target switch (cases 0/1/2) falls straight through to the epilogue: case 0's body IS the implicit-default fld1, shared with the no-match fall-through. The trailing `return 1.0f;` below must be folded into case 0 (matcher job). Full diff embedded; report in structure/character_dispersion-get_broken_hands_penalty.md
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

	// STRUCTURE DIFF:
	// target: 0x585e50            base: 0x454320
	// ; float survarium::character_dispersion_calculator::get_broken_hands_penalty(const unsigned char, const bool) const ; target 8 stmts / base 11 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x015 <0x18> | 0x015 <0x1a> | switch ( broken_hands_count )   SIZE
	// .. same ..
	// 0x05c <0x2b> | 0x05e <0x2d> | return using_double_handed_weapon ? m_params->injury_penalty_for_double_handed : m_params->injury_penalty_for_one_handed;   SIZE
	// --          | <0>         |    EMPTY only base
	// --          | 0x08b <0x2> | return 1.0f;   ONLY base
	// .. same ..
	// ; aligned 6, size-diffs 2, quantity-diffs 3
	// VERDICT: STRUCTURE MISMATCH (both) - base has an extra trailing `return 1.0f;`; fold it into `case 0` so the switch falls through to the shared epilogue fld1  trail: character_dispersion-get_broken_hands_penalty.md
}

} // namespace survarium
