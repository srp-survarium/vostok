////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/client_player_update.h>

namespace survarium {

// STATE[BLOCKED]: body matched from asm (rva 0x781199) but cannot be diffed -
// the udp_match_packet type cluster does not compile yet (see header note in
// network_core/udp_match_packet.h). The three target statements are, in order:
//     input.serialize( packet );   -> player_input::serialize  (this+0x00)
//     state.serialize( packet );   -> player_state::serialize  (this+0x14)
//     packet.append( time_in_ms ); -> packet<udp_match_packet>::append( u32 ),
//                                      time_in_ms at this+0x58
// Unblocks together with player_input::serialize / player_state::serialize /
// inventory_item::serialize, which sit on the same blocker. Body left empty so
// the module keeps building.
// void survarium::client_player_update::serialize(vostok::network_core::udp_match_packet&) const
void client_player_update::serialize( network_core::udp_match_packet& packet ) const
{
	// STRUCTURE DIFF:
	// target: 0x771190            base: 0x44d2e0
	// ; void survarium::client_player_update::serialize(vostok::network_core::udp_match_packet&) const ; target 3 stmts / base 0 stmts
	// 0x009 <0xc> | --          | L23   ONLY target
	// 0x015 <0xf> | --          | L24   ONLY target
	// 0x024 <0xf> | --          | L25   ONLY target
	// ; aligned 0, size-diffs 0, quantity-diffs 3
	// VERDICT: STRUCTURE MISMATCH (quantity) - base is empty stub, body blocked on udp_match_packet cluster; write 3 stmts when blocker clears  trail: client_player_update-serialize.md
}

} // namespace survarium
