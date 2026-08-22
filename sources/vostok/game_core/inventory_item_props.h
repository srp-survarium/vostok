////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INVENTORY_ITEM_PROPS_H_INCLUDED
#define INVENTORY_ITEM_PROPS_H_INCLUDED

namespace survarium {

struct inventory_item_props {
	inline	inventory_item_props( ) : m_dict_id( 0 ), use_in_percents( u8(-1) ), m_amount( u16(-1) ) { }

public:
	/* 0x0000 */	u16		m_dict_id;
	/* 0x0002 */	union {
						u8		cooldown;
						u8		use_in_percents;
					};
	/* 0x0004 */	union {
						u16		m_amount;
						u32		m_amount_ms;
					};
}; // struct inventory_item_props

STATIC_SIZE_ASSERT(inventory_item_props, 0x8);

} // namespace survarium

#endif // #ifndef INVENTORY_ITEM_PROPS_H_INCLUDED
