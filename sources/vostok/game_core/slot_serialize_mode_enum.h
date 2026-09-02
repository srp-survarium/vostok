// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef SLOT_SERIALIZE_MODE_ENUM_H_INCLUDED
#define SLOT_SERIALIZE_MODE_ENUM_H_INCLUDED

namespace survarium {

enum slot_serialize_mode_enum
{
	serialize_just_condition_stack_values	= 0x0,
	serialize_just_amount_values			= 0x1,
	serialize_both_values					= 0x2,
};

} // namespace survarium

#endif // #ifndef SLOT_SERIALIZE_MODE_ENUM_H_INCLUDED
