// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PROFILE_SLOT_RESTRICTION_H_INCLUDED
#define PROFILE_SLOT_RESTRICTION_H_INCLUDED

namespace survarium {

struct profile_slot_restriction {

public:
	/* 0x0000 */	u8		slot_dict_id;
	/* 0x0001 */	u8		category_dict_id;
}; // struct profile_slot_restriction

STATIC_SIZE_ASSERT(profile_slot_restriction, 0x2);

} // namespace survarium

#endif // #ifndef PROFILE_SLOT_RESTRICTION_H_INCLUDED
