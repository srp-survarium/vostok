// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef SERVER_PLAYER_UPDATE_H_INCLUDED
#define SERVER_PLAYER_UPDATE_H_INCLUDED

#include <vostok/game_core/player_input.h>
#include <vostok/game_core/player_state.h>
#include <vostok/game_core/weapon_state.h>

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

struct server_player_update {
			void	deserialize			( network_core::packet_reader& packet );
	inline	void	serialize			( network_core::udp_match_packet& arg_0 ) const { /* no source */ }

public:
	/* 0x0000 */	player_input	input;
	/* 0x0014 */	player_state	state;
	/* 0x0058 */	weapon_state	weapon_state;
}; // struct server_player_update

STATIC_SIZE_ASSERT(server_player_update, 0x5C);

} // namespace survarium

#endif // #ifndef SERVER_PLAYER_UPDATE_H_INCLUDED
