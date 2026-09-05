// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROFILE_SLOT_H_INCLUDED
#define PROFILE_SLOT_H_INCLUDED

#include <vostok/game_core/inventory_item_instance.h>

namespace survarium {

struct profile_slot {
public:
	/* 0x0000 */	inventory_item_instance		item;

	// sushi@TODO: writer-forwarding model; the retained profile consumer proves only the reader seam.
	inline	void	serialize	( network_core::udp_match_packet& packet, slot_serialize_mode_enum mode ) const
	{
		item.serialize( packet, mode );
	}
	inline	void	deserialize	( network_core::packet_reader& reader, slot_serialize_mode_enum mode )
	{
		item.deserialize( reader, mode );
	}
}; // struct profile_slot

STATIC_SIZE_ASSERT(profile_slot, 0x10);

} // namespace survarium

#endif // #ifndef PROFILE_SLOT_H_INCLUDED
