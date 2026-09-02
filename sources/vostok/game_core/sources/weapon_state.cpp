// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/weapon_state.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

weapon_state::weapon_state( )
{
}

weapon_state& weapon_state::operator=( weapon_state const& other )
{
	// claude@NOTE: target does a plain member-wise copy with NO self-guard.
	slot_id			= other.slot_id;
	ammo_slot_id	= other.ammo_slot_id;
	state			= other.state;
	return *this;
}

// claude@NOTE: structure is correct (three bool reads -> u8 members @0/1/2, order matches
// target asm). Residual is the inline-vs-call wall: target CALLs out-of-line
// packet_reader::r<bool>() COMDATs while game_core's /Od /Ob2 /GL inlines our header's
// r<T>() (delegates to r(void*,u32,u32)) into the caller. Not forceable here - fix lives in
// network_core/packet_reader_inline.h. See patterns/inline-vs-call-template-comdat.md.
void weapon_state::deserialize( network_core::packet_reader& packet )
{
	slot_id			= packet.r< bool >( );
	ammo_slot_id	= packet.r< bool >( );
	state			= packet.r< bool >( );
}

} // namespace survarium
