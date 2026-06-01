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

// STATE[95.74%|PARTIAL]: switch body byte-exact (leaf m_params reads + ternary temps
// match instruction-for-instruction). The prologue `mov byte[ebp-1],0;
// lea eax,[ebp-1]; call empty_stub` (delinker misnames it `finalize_impl`) is a
// compiled-out ASSERT recovered below; residual is the switch dispatch range check
// (target jmp [edx*4+table] direct vs base cmp/ja). Same prologue in get_broken_hands_penalty.
//
// claude@NOTE: jump table (read from .text relocs of the target obj) maps
//   case type_stand(0)->0x25, type_crouch(1)->0x8b, type_sprint(2)->0xef,
//   type_jump(3)->0xf9, type_preview(4)->0x103(default 1.0f).
//   Member offsets verified against character_dispersion_params layout:
//   idle@0 idle_aim@4 walk@8 walk_aim@0xc run@0x10 jump@0x14 crouch@0x18
//   crouch_aim@0x1c crouch_walk@0x20 crouch_walk_aim@0x24.
float character_dispersion_calculator::get_target_koef( weapon_user_state_enum character_state, bool is_moving, bool is_aiming ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) ); // compiled-out: recovers `call empty_stub` (+0x0c)

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

	// FUNCTION BODY[0x595ee0]
	// <0x595ee9>|0x009|+0x00c:'89'	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
	// <0x595ef5>|0x015|+0x010		switch ( character_state ) {
	// <0x595f05>|0x025			case type_stand: if ( is_moving )
	// <0x595f0d>|0x02d				return is_aiming ? walk_aim : walk;
	// <0x595f3f>|0x05f				else return is_aiming ? idle_aim : idle;
	// <0x595f6b>|0x08b			case type_crouch: if ( is_moving )
	// <0x595f73>|0x093				return is_aiming ? crouch_walk_aim : crouch_walk;
	// <0x595fa2>|0x0c2				else return is_aiming ? crouch_aim : crouch;
	// <0x595fcf>|0x0ef			case type_sprint: return run;
	// <0x595fd9>|0x0f9			case type_jump:   return jump;
	// <0x595fe3>|0x103			default:          return 1.0f;
	// ******
}

// STATE[93.33%|PARTIAL]: switch body byte-exact (case 0/default -> fld1; cases 1/2
// are ternaries reading m_params->injury_penalty_*). The prologue `mov byte[ebp-1],0;
// lea eax,[ebp-1]; call empty_stub` (delinker misnames it `finalize_impl`) is a
// compiled-out ASSERT recovered below (same prologue as get_target_koef); residual
// is the switch dispatch shape.
//
// claude@NOTE: switch over broken_hands_count. case 0 returns 1.0f via `fld1`; the
//   implicit default returns 1.0f too and MSVC folds the two identical returns into
//   ONE `fld1` reached by both the je-0 and the fall-through. case 1's 1.0f arm is a
//   const load (target .data `clear_value` = 1.0f). Offsets verified vs
//   character_dispersion_params: injury_penalty_for_double_handed@0x30,
//   injury_penalty_for_one_handed@0x34.
// claude@MATCH: `case 0: return 1.0f;` (NOT `case 0: break;`) is required - `break`
//   folds case 0 into the default path and drops the explicit `cmp 0` the target has
//   (and left objdiff unable to score the function, reporting None).
float character_dispersion_calculator::get_broken_hands_penalty( u8 broken_hands_count, bool using_double_handed_weapon ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) ); // compiled-out: recovers `call empty_stub` (+0x0c)

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

	// FUNCTION BODY[0x595e50]
	// <0x595e59>|0x009|+0x00c:'130'	ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
	// <0x595e65>|0x015|+0x018		switch ( broken_hands_count ) {
	// <0x595e7d>|0x02d			case 0: return 1.0f;
	// <0x595e81>|0x031			case 1: return double_handed ? injury_double : 1.0f;
	// <0x595eac>|0x05c			case 2: return double_handed ? injury_double : injury_one;
	// ******
}

} // namespace survarium
