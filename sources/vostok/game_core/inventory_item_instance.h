// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INVENTORY_ITEM_INSTANCE_H_INCLUDED
#define INVENTORY_ITEM_INSTANCE_H_INCLUDED

#include <vostok/game_core/slot_serialize_mode_enum.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

struct inventory_item_instance {
public:
	inline	explicit	inventory_item_instance	( ) :
		condition_or_stack	( 0 ),
		amount_in_inventory	( 0 ),
		id					( 0 ),
		dict_id				( 0 )
	{}

	inline	void		serialize				( network_core::udp_match_packet& arg_0, slot_serialize_mode_enum arg_1 ) const { /* no source */ }
	inline	void		deserialize				( network_core::packet_reader& reader, slot_serialize_mode_enum mode )
	{
		dict_id	= reader.r< u16 >( );
		id		= reader.r< u32 >( );

		if ( mode != serialize_just_amount_values )
			condition_or_stack	= reader.r< u16 >( );

		if ( mode != serialize_just_condition_stack_values )
			amount_in_inventory	= reader.r< u32 >( );
	}


public:
	/* 0x0000 */	u32		condition_or_stack;
	/* 0x0004 */	u32		amount_in_inventory;
	/* 0x0008 */	u32		id;
	/* 0x000c */	u16		dict_id;
}; // struct inventory_item_instance

STATIC_SIZE_ASSERT(inventory_item_instance, 0x10);

inline bool operator==( inventory_item_instance const& item, const u32 id )
{
	return item.id == id;
}

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_INSTANCE_H_INCLUDED
