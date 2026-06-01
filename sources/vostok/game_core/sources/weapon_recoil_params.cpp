////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_recoil_params.h>

namespace survarium {

// STATE[100%|DONE]: body matches target 1:1 (12 movss stores, offsets
// 0x00-0x1C then 0x24-0x30, offset 0x20 never stored); the #107 18% was an
// inadequate anchor that let LTCG dead-store-eliminate the member stores. The
// retry (PR this branch) escapes an OBSERVED instance through the opaque sink
// in temp_include_all (use_game_core_weapon_recoil_params), so the stores are
// kept -> 100%.
// survarium::weapon_recoil_params::weapon_recoil_params()
// claude@MATCH: member-init list in declaration order; additive_back_recoil
// (offset 0x20) is deliberately omitted - the target never stores it (no movss
// [this+20h] in the asm). additive_recoil_time gets math::epsilon_3 (the only
// non-zero constant; delinker resolved the rdata symbol as epsilon_3 = .001f).
weapon_recoil_params::weapon_recoil_params( ) :
	first_shoot_side_recoil		( 0.0f ),
	shoot_side_recoil		( 0.0f ),
	first_shoot_back_recoil		( 0.0f ),
	shoot_back_recoil		( 0.0f ),
	shoot_recoil_min_angle		( 0.0f ),
	shoot_recoil_angle_range	( 0.0f ),
	additive_recoil_time		( math::epsilon_3 ),
	additive_side_recoil		( 0.0f ),
	additive_recoil_min_angle	( 0.0f ),
	additive_recoil_angle_range	( 0.0f ),
	side_compensation_speed		( 0.0f ),
	back_compensation_speed		( 0.0f )
{
}

// STATE[STUB]
// survarium::weapon_recoil_params::weapon_recoil_params(vostok::configs::binary_config_value const&)
weapon_recoil_params::weapon_recoil_params( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
	// <0x593228>|0x098|+0x014:'40'
	// <0x59323c>|0x0ac|+0x019:'41'
	// <0>
	// <0x593255>|0x0c5|+0x014:'43'
	// <0x593269>|0x0d9|+0x01a:'44'
	// <0>
	// <0x593283>|0x0f3|+0x014:'46'
	// <0x593297>|0x107|+0x01a:'47'
	// <0>
	// <0x5932b1>|0x121|+0x014:'49'
	// <0x5932c5>|0x135|+0x01a:'50'
	// <0>
	// <0x5932df>|0x14f|+0x028:'52'
	// <0>
	// <0x593307>|0x177|+0x01a:'54'
	// <0x593321>|0x191|+0x022:'55'
	// <0>
	// <1>
	// <0x593343>|0x1b3|+0x014:'58'
	// <0x593357>|0x1c7|+0x01a:'59'
	// <0>
	// <0x593371>|0x1e1|+0x014:'61'
	// <0x593385>|0x1f5|+0x01a:'62'
	// <0>
	// <0x59339f>|0x20f|+0x014:'64'
	// <0x5933b3>|0x223|+0x01a:'65'
	// <0>
	// <1>
	// <0x5933cd>|0x23d|+0x028:'68'
	// <0>
	// <0x5933f5>|0x265|+0x01a:'70'
	// <0x59340f>|0x27f|+0x022:'71'
	// <0>
	// <1>
	// <0x593431>|0x2a1|+0x014:'74'
	// <0x593445>|0x2b5|+0x01a:'75'
	// <0>
	// <0x59345f>|0x2cf|+0x014:'77'
	// <0x593473>|0x2e3|+0x01a:'78'
	// ******
}

} // namespace survarium
