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
}

} // namespace survarium
