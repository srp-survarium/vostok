// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.01.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PROFILE_SLOTS_H_INCLUDED
#define PROFILE_SLOTS_H_INCLUDED

namespace survarium {

#define WEAPON_COUNT	2
#define AMMO_COUNT		4
#define ITEM_COUNT		13

extern profile_slot_enum weapon_slots[WEAPON_COUNT];
extern profile_slot_enum ammunition_slots[AMMO_COUNT];
extern profile_slot_enum item_slots[ITEM_COUNT];

} // namespace survarium

#endif // #ifndef PROFILE_SLOTS_H_INCLUDED
