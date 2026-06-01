////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_state.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_state::weapon_state()
weapon_state::weapon_state( )
{
	// FUNCTION BODY
	// <0x5a0c40>|0x000|+0x007:'15'	{
	// <0x5a0c47>|0x007|      :'16'	}
	// ******
}

// STATE[STUB]
// survarium::weapon_state& survarium::weapon_state::operator=(survarium::weapon_state const&)
weapon_state& weapon_state::operator=( weapon_state const& other )
{
	// FUNCTION BODY
	// <0x5a0c07>|0x007|+0x00a:'27'
	// <0x5a0c11>|0x011|+0x00c:'28'
	// <0x5a0c1d>|0x01d|+0x00c:'29'
	// <0>
	// <0x5a0c29>|0x029|+0x003:'31'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_state::deserialize(vostok::network_core::packet_reader&)
void weapon_state::deserialize( network_core::packet_reader& packet )
{
	// FUNCTION BODY
	// <0x5a0c59>|0x009|+0x00d:'36'
	// <0x5a0c66>|0x016|+0x00e:'37'
	// <0x5a0c74>|0x024|+0x00e:'38'
	// ******
}

} // namespace survarium
