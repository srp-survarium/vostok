// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_STATE_H_INCLUDED
#define WEAPON_STATE_H_INCLUDED

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}

namespace survarium {

struct weapon_state {
			explicit			weapon_state( );
							weapon_state( weapon_state const& other );

			weapon_state&		operator=	( weapon_state const& other );

			void				deserialize	( network_core::packet_reader& packet );
	inline	void				serialize	( network_core::udp_match_packet& arg_0 ) const { /* no source */ }


public:
	/* 0x0000 */	u8		slot_id;
	/* 0x0001 */	u8		ammo_slot_id;
	/* 0x0002 */	u8		state;
}; // struct weapon_state

STATIC_SIZE_ASSERT(weapon_state, 0x3);

} // namespace survarium

#endif // #ifndef WEAPON_STATE_H_INCLUDED
