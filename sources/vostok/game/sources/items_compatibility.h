// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ITEMS_COMPATIBILITY_H_INCLUDED
#define ITEMS_COMPATIBILITY_H_INCLUDED

namespace survarium {

struct items_compatibility {

public:
	/* 0x0000 */	u16		first_item_dict_id;
	/* 0x0002 */	u16		second_item_dict_id;
}; // struct items_compatibility

STATIC_SIZE_ASSERT(items_compatibility, 0x4);

} // namespace survarium

#endif // #ifndef ITEMS_COMPATIBILITY_H_INCLUDED
