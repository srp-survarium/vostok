////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "character_dispersion_calculator.h"

namespace survarium {

// STATE[STUB]
// survarium::character_dispersion_calculator::character_dispersion_calculator()
character_dispersion_calculator::character_dispersion_calculator( )
{
}

// STATE[STUB]
// void survarium::character_dispersion_calculator::set_character_dispersion_params(survarium::character_dispersion_params const*)
void character_dispersion_calculator::set_character_dispersion_params( character_dispersion_params const* character_params )
{
	// FUNCTION BODY
	// <0x595e07>|0x000|0x000:'28'
	// <0x595e0f>|0x008|0x008:'29'
	// <0x595e17>|0x010|0x008:'30'
	// <0x595e24>|0x01d|0x00d:'31'
	// <0x595e31>|0x02a|0x00d:'32'
	// 1
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
	u32							current_time_in_ms)
{
	// LOCALS
	// float 						dt
	// ******

	// FUNCTION BODY
	// <0x596099>|0x000|0x000:'50'
	// <0x5960a5>|0x00c|0x00c:'51'
	// <0x5960ae>|0x015|0x009:'52'
	// <0x5960b7>|0x01e|0x009:'53'
	// 1
	// 2
	// <0x5960bc>|0x023|0x005:'56'
	// <0x5960c7>|0x02e|0x00b:'57'
	// 1
	// <0x5960cc>|0x033|0x005:'59'
	// <0x5960eb>|0x052|0x01f:'60'
	// 1
	// 2
	// <0x5960f4>|0x05b|0x009:'63'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x596128>|0x08f|0x034:'69'
	// 1
	// 2
	// 3
	// <0x596156>|0x0bd|0x02e:'73'
	// 1
	// <0x596167>|0x0ce|0x011:'75'
	// 1
	// 2
	// <0x596197>|0x0fe|0x030:'78'
	// 1
	// <0x5961a8>|0x10f|0x011:'80'
	// 1
	// 2
	// 3
	// 4
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
	// <0x595ee9>|0x000|0x000:'89'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// <0x595ef5>|0x00c|0x00c:'104'
	// 1
	// <0x595f05>|0x01c|0x010:'106'
	// <0x595f0d>|0x024|0x008:'107'
	// <0x595f3d>|0x054|0x030:'108'
	// <0x595f3f>|0x056|0x002:'109'
	// 1
	// 2
	// <0x595f6b>|0x082|0x02c:'112'
	// <0x595f73>|0x08a|0x008:'113'
	// <0x595fa0>|0x0b7|0x02d:'114'
	// <0x595fa2>|0x0b9|0x002:'115'
	// 1
	// 2
	// <0x595fcf>|0x0e6|0x02d:'118'
	// 1
	// <0x595fd9>|0x0f0|0x00a:'120'
	// 1
	// <0x595fe3>|0x0fa|0x00a:'122'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// float survarium::character_dispersion_calculator::get_broken_hands_penalty(const unsigned char, const bool) const
float character_dispersion_calculator::get_broken_hands_penalty( u8 broken_hands_count, bool using_double_handed_weapon ) const
{
	return 0.0f;
	// FUNCTION BODY
	// <0x595e59>|0x000|0x000:'130'
	// <0x595e65>|0x00c|0x00c:'131'
	// 1
	// <0x595e7d>|0x024|0x018:'133'
	// 1
	// <0x595e81>|0x028|0x004:'135'
	// 1
	// <0x595eac>|0x053|0x02b:'137'
	// 1
	// 2
	// 3
	// ******
}

} // namespace survarium
