// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef ITEM_TYPES_ENUM_H_INCLUDED
#define ITEM_TYPES_ENUM_H_INCLUDED

namespace survarium {

enum item_types_enum {
	item_type_base_medkit		= 0x0,
	item_type_oxygen_tank		= 0x1,
	item_type_base_trap			= 0x2,
	item_type_artefact_lifebone = 0x3,
	item_type_foo				= 0x4,
	item_type_rifle_scope		= 0x5,
	item_types_count			= 0x6
};

} // namespace survarium

#endif // #ifndef ITEM_TYPES_ENUM_H_INCLUDED
