// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef INVENTORY_SLOT_H_INCLUDED
#define INVENTORY_SLOT_H_INCLUDED

#include <vostok/game_core/inventory_item.h>

namespace survarium {

struct inventory_slot {
public:
	/* 0x0000 */	inventory_item_ptr	item;
}; // struct inventory_slot

STATIC_SIZE_ASSERT(inventory_slot, 0x4);

} // namespace survarium

#endif // #ifndef INVENTORY_SLOT_H_INCLUDED
