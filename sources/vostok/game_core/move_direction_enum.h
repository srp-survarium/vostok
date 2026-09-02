// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef MOVE_DIRECTION_ENUM_H_INCLUDED
#define MOVE_DIRECTION_ENUM_H_INCLUDED

namespace survarium {

enum move_direction_enum {
	move_direction_on_site		= 0x0000,
	move_direction_fwd			= 0x0001,
	move_direction_fwd_right	= 0x0002,
	move_direction_right		= 0x0003,
	move_direction_back_right	= 0x0004,
	move_direction_back			= 0x0005,
	move_direction_back_left	= 0x0006,
	move_direction_left			= 0x0007,
	move_direction_fwd_left		= 0x0008
};

} // namespace survarium

#endif // #ifndef MOVE_DIRECTION_ENUM_H_INCLUDED
