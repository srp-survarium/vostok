////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_ITEM_INSTANCE_H_INCLUDED
#define INVENTORY_ITEM_INSTANCE_H_INCLUDED

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

enum slot_serialize_mode_enum {};

struct inventory_item_instance {
public:
	inline	explicit	inventory_item_instance	( ) { /* no source */ }

	inline	void		serialize				( network_core::udp_match_packet& arg_0, slot_serialize_mode_enum arg_1 ) const { /* no source */ }
	inline	void		deserialize				( network_core::packet_reader& arg_0, slot_serialize_mode_enum arg_1 ) { /* no source */ }


public:
	/* 0x0000 */	u32		condition_or_stack;
	/* 0x0004 */	u32		amount_in_inventory;
	/* 0x0008 */	u32		id;
	/* 0x000c */	u16		dict_id;
}; // struct inventory_item_instance

STATIC_SIZE_ASSERT(inventory_item_instance, 0x10);

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_INSTANCE_H_INCLUDED
