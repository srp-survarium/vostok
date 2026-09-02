// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/server_player_update.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

void server_player_update::deserialize( network_core::packet_reader& packet )
{
	input.deserialize			( packet );
	state.deserialize			( packet );
	weapon_state.deserialize	( packet );
}

} // namespace survarium
