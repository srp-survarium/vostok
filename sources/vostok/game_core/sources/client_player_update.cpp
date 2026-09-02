// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/client_player_update.h>
#include <vostok/network_core/udp_match_packet.h>

namespace survarium {

// claude@NOTE: structure is correct (3 stmts in order; input/state serialize CALLs match
// byte-for-byte). Residual is the inline-vs-call wall on the write side: target keeps
// packet<udp_match_packet>::append(u32) as an out-of-line COMDAT and CALLs it (pushes the
// u32 value directly), while game_core's /Od /Ob2 /GL inlines our header's append(u32)
// (which delegates to append(pcvoid,u32)) into this caller (spills to a local, push 4 /
// lea / push). Same wall as the deserialize r<T> COMDATs; fix lives in
// network_core/packet_inline.h (different unit). See patterns/inline-vs-call-template-comdat.md.
void client_player_update::serialize( network_core::udp_match_packet& packet ) const
{
	input.serialize		( packet );
	state.serialize		( packet );
	packet.append		( time_in_ms );
}

} // namespace survarium
