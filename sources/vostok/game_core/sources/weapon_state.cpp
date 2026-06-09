////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_state.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
weapon_state::weapon_state( )
{
}

// STATE[100%|DONE]
weapon_state& weapon_state::operator=( weapon_state const& other )
{
	// claude@NOTE: target does a plain member-wise copy with NO self-guard.
	slot_id			= other.slot_id;
	ammo_slot_id	= other.ammo_slot_id;
	state			= other.state;
	return *this;
}

// STATE[PARTIAL]: reads slot_id, ammo_slot_id, state - each as r< bool > (single-byte
// stores into the three u8 members, matching the target's r<bool> instantiation).
void weapon_state::deserialize( network_core::packet_reader& packet )
{
	slot_id			= packet.r< bool >( );
	ammo_slot_id	= packet.r< bool >( );
	state			= packet.r< bool >( );

	// STRUCTURE DIFF[target 0x590c50 | base 0x453a20]: target 3 / base 3 stmts
	//   1: 0x009 <0xd> | 0x009 <0x22> | slot_id			= packet.r< bool >( );   SIZE
	//   2: 0x016 <0xe> | 0x02b <0x23> | ammo_slot_id	= packet.r< bool >( );   SIZE
	//   3: 0x024 <0xe> | 0x04e <0x23> | state			= packet.r< bool >( );   SIZE
	// ; aligned 0, size-diffs 3, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - all 3 SIZE are packet_reader::r<bool> LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
