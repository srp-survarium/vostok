// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SLOT_DEF_H_INCLUDED
#define SLOT_DEF_H_INCLUDED

// the canonical dump re-prints profile_slot_enum here; the enum lives in game_core
#include <vostok/game_core/profile_slot_enum.h>

namespace survarium {

struct slot_def {

public:
	/* 0x0000 */	profile_slot_enum		slot;
	/* 0x0004 */	pcstr					table_name;
}; // struct slot_def

STATIC_SIZE_ASSERT(slot_def, 0x8);

} // namespace survarium

#endif // #ifndef SLOT_DEF_H_INCLUDED
