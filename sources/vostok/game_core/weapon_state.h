// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_STATE_H_INCLUDED
#define WEAPON_STATE_H_INCLUDED

#include <vostok/network_core/udp_match_packet.h>

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}

namespace survarium {

struct weapon_state {
			explicit			weapon_state( );
	// sushi@TODO: retain implicit copying; the old undefined copy declaration has no target record.

			weapon_state&		operator=	( weapon_state const& other );

			void				deserialize	( network_core::packet_reader& packet );
	// sushi@TODO: inverse of the retained reader; verify the original sending consumer.
	inline	void				serialize	( network_core::udp_match_packet& packet ) const
	{
		packet.append	( slot_id );
		packet.append	( ammo_slot_id );
		packet.append	( state );
	}


public:
	/* 0x0000 */	u8		slot_id;
	/* 0x0001 */	u8		ammo_slot_id;
	/* 0x0002 */	u8		state;
}; // struct weapon_state

STATIC_SIZE_ASSERT(weapon_state, 0x3);

} // namespace survarium

#endif // #ifndef WEAPON_STATE_H_INCLUDED
