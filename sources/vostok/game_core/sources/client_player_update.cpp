// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/client_player_update.h>
#include <vostok/network_core/udp_match_packet.h>

namespace survarium {

void client_player_update::serialize( network_core::udp_match_packet& packet ) const
{
	input.serialize		( packet );
	state.serialize		( packet );
	packet.append		( time_in_ms );
}

} // namespace survarium
