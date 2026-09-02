// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef DICTIONARY_ITEM_H_INCLUDED
#define DICTIONARY_ITEM_H_INCLUDED

namespace survarium {

struct dictionary_item {
	/* 0x0000 */	u32								item_id;
	/* 0x0004 */	configs::binary_config_ptr		item_cfg;
	/* 0x0008 */	fixed_string<260>				item_cfg_name;
	/* 0x0118 */	u8								item_category;
	/* 0x0119 */	u8								combat_log_icon;
	/* 0x011a */	bool							is_premium;
	/* 0x011b */	bool							is_stack;
	/* 0x011c */	float							weight;

	// STATE[STUB]: Defined in `items_dictionary_cook.h`.
			bool	is_ammo			( )
	{
		return item_category == 9 // sushi@TODO: There should be an enum for that
			|| item_category == 18
			|| item_category == 19
			|| item_category == 20
			|| item_category == 21;
	}
}; // struct dictionary_item

STATIC_SIZE_ASSERT(dictionary_item, 0x120);

} // namespace survarium

#endif // #ifndef DICTIONARY_ITEM_H_INCLUDED
