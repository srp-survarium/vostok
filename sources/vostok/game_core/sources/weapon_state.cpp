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

// STATE[8.26%|PARTIAL]: reads slot_id, ammo_slot_id, state - each as r< bool > (single-byte
// stores into the three u8 members, matching the target's r<bool> instantiation). The low % is
// calling-convention-wide: the target CALLS the LTCG-folded custom-convention r<bool> COMDAT
// (0x7e950: reader in edx, result in al, no stack args), while the /Od base inlines the r<bool>
// wrapper and calls the inner 3-arg r(void*,u32,u32) per read - every instruction differs but
// the 3-statement shape is identical, non-steerable.
void weapon_state::deserialize( network_core::packet_reader& packet )
{
	slot_id			= packet.r< bool >( );
	ammo_slot_id	= packet.r< bool >( );
	state			= packet.r< bool >( );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0xb|0x590c59|0x4668a9|0xd |0x18|0   |slot_id			= packet.r< bool >( );
	// SIZE +0xb|0x590c66|0x4668c1|0xe |0x19|+1  |ammo_slot_id	= packet.r< bool >( );
	// SIZE +0xb|0x590c74|0x4668da|0xe |0x19|+2  |state			= packet.r< bool >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - 3/3, uniform +0xb per row = target's folded r<bool>
	//   call vs base's inlined wrapper + inner r() call, non-steerable.
}

} // namespace survarium
