////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/client_player_update.h>
#include <vostok/network_core/udp_match_packet.h>

namespace survarium {

// STATE[85.24%|PARTIAL]: input.serialize, state.serialize, append(time_in_ms) - 3-stmt
// shape matches target (rva 0x771190).
void client_player_update::serialize( network_core::udp_match_packet& packet ) const
{
	input.serialize		( packet );
	state.serialize		( packet );
	packet.append		( time_in_ms );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0x8|0x7711b4|0x590574|0xf |0x17|0   |packet.append		( time_in_ms );
	// VERDICT: STRUCTURE MATCH (shape ok) - input/state serialize forwards align clean; sole SIZE is packet<T>::append LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
