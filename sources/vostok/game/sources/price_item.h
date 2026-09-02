// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PRICE_ITEM_H_INCLUDED
#define PRICE_ITEM_H_INCLUDED

namespace survarium {

struct price_item {

public:
	/* 0x0000 */	u16		item_dict_id;
	/* 0x0002 */	u16		cost;
	/* 0x0004 */	u8		reputation_level;
}; // struct price_item

STATIC_SIZE_ASSERT(price_item, 0x6);

} // namespace survarium

#endif // #ifndef PRICE_ITEM_H_INCLUDED
