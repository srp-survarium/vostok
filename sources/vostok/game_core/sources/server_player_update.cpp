////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/server_player_update.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[PARTIAL]: input.deserialize, state.deserialize, weapon_state.deserialize -
// 3-stmt shape matches target.
void server_player_update::deserialize( network_core::packet_reader& packet )
{
	input.deserialize			( packet );
	state.deserialize			( packet );
	weapon_state.deserialize	( packet );

	// STRUCTURE DIFF[target 0x7711d0 | base 0x5851c0]: target 3 / base 3 stmts
	// .. same ..
	// ; aligned 3, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH - 3 deserialize forwards, byte-clean structure (no SIZE/quantity diffs).
}

} // namespace survarium
