// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef BODY_PART_PARAMETERS_MODIFYER_H_INCLUDED
#define BODY_PART_PARAMETERS_MODIFYER_H_INCLUDED

#include <vostok/game_core/hit_type_parameters_modifyer.h>

namespace survarium {

struct body_part_parameters_modifyer {
	/* 0x0000 */	float			health;
	/* 0x0004 */	float			health_regeneration;
	/* 0x0008 */	map< fixed_string<16>, hit_type_parameters_modifyer >	hit_type_modifyers;
}; // struct body_part_parameters_modifyer

STATIC_SIZE_ASSERT(body_part_parameters_modifyer, 0x20);

} // namespace survarium

#endif // #ifndef BODY_PART_PARAMETERS_MODIFYER_H_INCLUDED
