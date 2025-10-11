////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_base_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_base_state::weapon_core_base_state(survarium::weapon_core&, bool)
weapon_core_base_state::weapon_core_base_state( weapon_core& weapon, bool serialize_animation_state )
{
}

// STATE[STUB]
// bool survarium::weapon_core_base_state::deserializing() const
bool weapon_core_base_state::deserializing( ) const
{
	return false;
	// FUNCTION BODY
	// <0x6fce69>|0x000|0x000:'28'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_base_state::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_base_state::serialize( vostok::network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// vostok::animation::animation_playback_state playback_state<1>
	// base_player const& 			user<1>
	// bool 						success<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fcef5><1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ebf47 }, type_index: TypeIndex(0x918d) })
	// ******

	// FUNCTION BODY
	// <0x6fcee9>|0x000|0x000:'33'
	// 1
	// 2
	// 3
	// <0x6fcefb>|0x012|0x012:'37'
	// <0x6fcf0f>|0x026|0x014:'38'
	// <0x6fcf1b>|0x032|0x00c:'39'
	// 1
	// 2
	// <0x6fcf2c>|0x043|0x011:'42'
	// <0x6fcf4c>|0x063|0x020:'43'
	// 1
	// 2
	// <0x6fcf54>|0x06b|0x008:'46'
	// <0x6fcf5b>|0x072|0x007:'47'
	// 1
	// 2
	// <0x6fcf68>|0x07f|0x00d:'50'
	// <0x6fcf74>|0x08b|0x00c:'51'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_base_state::deserialize(vostok::network_core::packet_reader&)
void weapon_core_base_state::deserialize( vostok::network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x6fce99>|0x000|0x000:'57'
	// 1
	// <0x6fcea7>|0x00e|0x00e:'59'
	// <0x6fceb8>|0x01f|0x011:'60'
	// 1
	// ******
}

} // namespace survarium
