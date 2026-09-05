// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLIENT_PLAYER_UPDATE_H_INCLUDED
#define CLIENT_PLAYER_UPDATE_H_INCLUDED

#include <vostok/game_core/player_input.h>
#include <vostok/game_core/player_state.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

struct client_player_update {
	// sushi@TODO: inverse of the retained writer; verify the original receiving consumer.
	inline	void	deserialize			( network_core::packet_reader& packet )
	{
		input.deserialize	( packet );
		state.deserialize	( packet );
		time_in_ms			= packet.r< u32 >( );
	}
			void	serialize			( network_core::udp_match_packet& packet ) const;

public:
	/* 0x0000 */	player_input	input;
	/* 0x0014 */	player_state	state;
	/* 0x0058 */	u32				time_in_ms;
}; // struct client_player_update

STATIC_SIZE_ASSERT(client_player_update, 0x5C);

} // namespace survarium

#endif // #ifndef CLIENT_PLAYER_UPDATE_H_INCLUDED
