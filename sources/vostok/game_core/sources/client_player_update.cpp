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
	// FUNCTION BODY
	// <0x781199>|0x009|+0x00c:'23'		input.serialize( packet );
	// <0x7811a5>|0x015|+0x00f:'24'		state.serialize( packet );
	// <0x7811b4>|0x024|+0x00f:'25'		packet.append( time_in_ms );
	// ******
}

} // namespace survarium
