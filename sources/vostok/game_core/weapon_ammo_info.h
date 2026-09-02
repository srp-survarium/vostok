// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_AMMO_INFO_H_INCLUDED
#define WEAPON_AMMO_INFO_H_INCLUDED

namespace survarium {

struct weapon_ammo_info {
	/* 0x0000 */	u32		ammo1_total;
	/* 0x0004 */	u32		ammo2_total;
	/* 0x0008 */	u32		ammo_in_magazine;
	/* 0x000c */	u16		fire_queue_size;
	/* 0x000e */	u8		current_ammo_type;
	/* 0x000f */	bool	round_is_chambered;
}; // struct weapon_ammo_info

STATIC_SIZE_ASSERT(weapon_ammo_info, 0x10);

} // namespace survarium

#endif // #ifndef WEAPON_AMMO_INFO_H_INCLUDED
