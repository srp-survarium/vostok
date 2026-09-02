// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROFILE_SLOT_ENUM_H_INCLUDED
#define PROFILE_SLOT_ENUM_H_INCLUDED

namespace survarium {

enum profile_slot_enum {
	helmet_slot        = 0x00,
	mask_slot          = 0x01,
	torso_slot         = 0x02,
	back_slot          = 0x03,
	pants_slot         = 0x04,
	gloves_slot        = 0x05,
	boots_slot         = 0x06,
	weapon1_slot       = 0x07,
	ammo1_weapon1_slot = 0x08,
	ammo2_weapon1_slot = 0x09,
	weapon2_slot       = 0x0a,
	ammo1_weapon2_slot = 0x0b,
	ammo2_weapon2_slot = 0x0c,
	quick_slot1        = 0x0d,
	quick_slot2        = 0x0e,
	quick_slot3        = 0x0f,
	quick_slot4        = 0x10,
	quick_slot5        = 0x11,
	quick_slot6        = 0x12,
	max_slots_count    = 0x13,
	invalid_slot       = 0x13
};

} // namespace survarium

#endif // #ifndef PROFILE_SLOT_ENUM_H_INCLUDED
