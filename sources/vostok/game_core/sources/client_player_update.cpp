////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/client_player_update.h>
#include <vostok/network_core/udp_match_packet.h>

namespace survarium {

// STATE[PARTIAL]: input.serialize, state.serialize, append(time_in_ms) - 3-stmt
// shape matches target (rva 0x771190).
void client_player_update::serialize( network_core::udp_match_packet& packet ) const
{
	input.serialize		( packet );
	state.serialize		( packet );
	packet.append		( time_in_ms );

	// STRUCTURE DIFF[target 0x771190 | base 0x56d8c0]: target 3 / base 3 stmts
	//   3: 0x024 <0xf> | 0x024 <0x17> | packet.append		( time_in_ms );   SIZE
	// ; aligned 2, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - input/state serialize forwards + append; sole SIZE is packet<T>::append LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
