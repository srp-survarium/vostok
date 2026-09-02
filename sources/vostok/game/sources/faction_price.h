// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FACTION_PRICE_H_INCLUDED
#define FACTION_PRICE_H_INCLUDED

namespace survarium {

struct price_item;

struct faction_price {

public:
	/* 0x0000 */	price_item*		items;
	/* 0x0004 */	u16				count;
	/* 0x0006 */	u8				faction_id;
}; // struct faction_price

STATIC_SIZE_ASSERT(faction_price, 0x8);

} // namespace survarium

#endif // #ifndef FACTION_PRICE_H_INCLUDED
