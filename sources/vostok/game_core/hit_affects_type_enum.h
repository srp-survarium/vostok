// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HIT_AFFECTS_TYPE_ENUM_H_INCLUDED
#define HIT_AFFECTS_TYPE_ENUM_H_INCLUDED

namespace survarium {

enum hit_affects_type_enum
{
  affects_type_death = 0x0,
  affects_type_bleeding = 0x1,
  affects_type_concussion = 0x2,
  affects_type_hand_damage = 0x3,
  affects_type_leg_damage = 0x4,
  affects_type_critical_poisoning = 0x5,
  affects_type_poisoning = 0x6,
  affects_type_radiation_sickness = 0x7,
  affects_type_blindness = 0x8,
  affect_types_count = 0x9,
};

static pcstr affects_captions[] = 
{
    "death",               // 0x0
    "bleeding",            // 0x1
    "concussion",          // 0x2
    "hand_damage",         // 0x3
    "leg_damage",          // 0x4
    "critical_poisoning",  // 0x5
    "poisoning",           // 0x6
    "radiation_sickness",  // 0x7
    "blindness"            // 0x8
};

static u32 affects_durations[] =
{
	10 , 10, 10,
	100, 10, 120,
	10 , 10, 10
};

} // namespace survarium

#endif // #define HIT_AFFECTS_TYPE_ENUM_H_INCLUDED
