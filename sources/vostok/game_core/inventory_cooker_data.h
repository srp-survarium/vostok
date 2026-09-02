// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef INVENTORY_COOKER_DATA_H_INCLUDED
#define INVENTORY_COOKER_DATA_H_INCLUDED

namespace survarium {

class damage_model;
class items_dictionary;
struct player_profile;

struct inventory_cooker_data {
	/* 0x0000 */	player_profile*			profile;
	/* 0x0004 */	items_dictionary*		dictionary;
	/* 0x0008 */	damage_model*			damage_model;
}; // struct inventory_cooker_data

STATIC_SIZE_ASSERT(inventory_cooker_data, 0xC);

} // namespace survarium

#endif // #ifndef INVENTORY_COOKER_DATA_H_INCLUDED
