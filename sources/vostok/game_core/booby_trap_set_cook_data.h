// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef BOOBY_TRAP_SET_COOK_DATA_H_INCLUDED
#define BOOBY_TRAP_SET_COOK_DATA_H_INCLUDED

namespace survarium {

struct booby_trap_set_cook_data {
	/* 0x0000 */	bool	is_local_player;
	/* 0x0001 */	u8		stack_size;
}; // struct booby_trap_set_cook_data

STATIC_SIZE_ASSERT(booby_trap_set_cook_data, 0x2);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_SET_COOK_DATA_H_INCLUDED
