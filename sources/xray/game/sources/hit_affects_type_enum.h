////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_AFFECTS_TYPE_ENUM_H_INCLUDED
#define HIT_AFFECTS_TYPE_ENUM_H_INCLUDED

namespace stalker2 {

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

} // namespace stalker2

#endif // #define HIT_AFFECTS_TYPE_ENUM_H_INCLUDED