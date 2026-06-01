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

// STATE[88%|PARTIAL]: switch body byte-exact (leaf m_params reads + ternary temps
// match instruction-for-instruction). Capped by the `empty_stub` inlined/stripped
// call prologue (`mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub`), which
// shifts the frame size 18h->1Ch and every [ebp-N] slot by 4 - an LTCG artifact
// not reproducible from this function's source (also present in get_broken_hands_penalty).
// float survarium::character_dispersion_calculator::get_target_koef(const survarium::weapon_user_state_enum, const bool, const bool) const
//
// claude@NOTE: jump table (read from .text relocs of the target obj) maps
//   case type_stand(0)->0x25, type_crouch(1)->0x8b, type_sprint(2)->0xef,
//   type_jump(3)->0xf9, type_preview(4)->0x103(default 1.0f).
//   Member offsets verified against character_dispersion_params layout:
//   idle@0 idle_aim@4 walk@8 walk_aim@0xc run@0x10 jump@0x14 crouch@0x18
//   crouch_aim@0x1c crouch_walk@0x20 crouch_walk_aim@0x24.
// claude@NOTE: target prologue has `mov byte[ebp-1],0; lea eax,[ebp-1];
//   call empty_stub` (also in get_broken_hands_penalty) - a stubbed/stripped
//   call the delinker could not resolve; not reproducible from source.
float character_dispersion_calculator::get_target_koef( weapon_user_state_enum character_state, bool is_moving, bool is_aiming ) const
{
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
}

// STATE[STUB]
// float survarium::character_dispersion_calculator::get_broken_hands_penalty(const unsigned char, const bool) const
float character_dispersion_calculator::get_broken_hands_penalty( u8 broken_hands_count, bool using_double_handed_weapon ) const
{
	return 0.0f;

	// FUNCTION BODY
	// <0x595e59>|0x009|+0x00c:'130'
	// <0x595e65>|0x015|+0x018:'131'
	// <0>
	// <0x595e7d>|0x02d|+0x004:'133'
	// <0>
	// <0x595e81>|0x031|+0x02b:'135'
	// <0>
	// <0x595eac>|0x05c|+0x02b:'137'
	// <0>
	// <1>
	// <2>
	// ******
}

} // namespace survarium
