// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_STATE_H_INCLUDED
#define PLAYER_STATE_H_INCLUDED

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}

namespace survarium {

struct player_state {
		void	deserialize	( network_core::packet_reader& packet );
		void	serialize	( network_core::udp_match_packet& packet ) const;

public:
	/* 0x0000 */	float4x4	transform;
	/* 0x0040 */	float		look_pitch;
}; // struct player_state

STATIC_SIZE_ASSERT(player_state, 0x44);

} // namespace survarium

#endif // #ifndef PLAYER_STATE_H_INCLUDED
