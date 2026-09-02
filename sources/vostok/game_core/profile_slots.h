// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROFILE_SLOTS_H_INCLUDED
#define PROFILE_SLOTS_H_INCLUDED

namespace survarium {

#define WEAPON_COUNT	2
#define AMMO_COUNT		4
#define ITEM_COUNT		13

static profile_slot_enum const weapon_slots[WEAPON_COUNT] = {
	weapon1_slot,
	weapon2_slot,
};

static profile_slot_enum const ammunition_slots[AMMO_COUNT] = {
	ammo1_weapon1_slot,
	ammo2_weapon1_slot,
	ammo1_weapon2_slot,
	ammo2_weapon2_slot,
};

static profile_slot_enum const item_slots[ITEM_COUNT] = {
	helmet_slot,
	mask_slot,
	torso_slot,
	back_slot,
	pants_slot,
	gloves_slot,
	boots_slot,
	quick_slot1,
	quick_slot2,
	quick_slot3,
	quick_slot4,
	quick_slot5,
	quick_slot6,
};

} // namespace survarium

#endif // #ifndef PROFILE_SLOTS_H_INCLUDED
