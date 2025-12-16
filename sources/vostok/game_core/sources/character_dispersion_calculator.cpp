////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/character_dispersion_calculator.h>

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

// STATE[STUB]
// float survarium::character_dispersion_calculator::get_target_koef(const survarium::weapon_user_state_enum, const bool, const bool) const
float character_dispersion_calculator::get_target_koef( weapon_user_state_enum character_state, bool is_moving, bool is_aiming ) const
{
	// STATICS
	// static <NoType> 				 = <0x595ff0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584f05 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584f6b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584fcf }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584fd9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x584fe3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x595ee9>|0x009|+0x00c:'89'
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
	// <0x595ef5>|0x015|+0x010:'104'
	// <0>
	// <0x595f05>|0x025|+0x008:'106'
	// <0x595f0d>|0x02d|+0x030:'107'
	// <0x595f3d>|0x05d|+0x002:'108'
	// <0x595f3f>|0x05f|+0x02c:'109'
	// <0>
	// <1>
	// <0x595f6b>|0x08b|+0x008:'112'
	// <0x595f73>|0x093|+0x02d:'113'
	// <0x595fa0>|0x0c0|+0x002:'114'
	// <0x595fa2>|0x0c2|+0x02d:'115'
	// <0>
	// <1>
	// <0x595fcf>|0x0ef|+0x00a:'118'
	// <0>
	// <0x595fd9>|0x0f9|+0x00a:'120'
	// <0>
	// <0x595fe3>|0x103|+0x004:'122'
	// <0>
	// <1>
	// <2>
	// ******
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
