// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AFFECT_TYPES_H_INCLUDED
#define AFFECT_TYPES_H_INCLUDED

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

enum affect_event_type_enum
{
	affect_applying = 0x0,
	affect_recalling = 0x1,
	affect_canceling = 0x2,
};

static pcstr affects_captions[] =
{
	"death",
	"bleeding",
	"concussion",
	"hand_damage",
	"leg_damage",
	"critical_poisoning",
	"poisoning",
	"radiation_sickness",
	"blindness"
};

static u32 affects_durations[] =
{
	10, 10, 10,
	100, 10, 120,
	10, 10, 10
};

} // namespace survarium

#endif // #ifndef AFFECT_TYPES_H_INCLUDED
