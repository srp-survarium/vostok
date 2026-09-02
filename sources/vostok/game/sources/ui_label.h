// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef UI_LABEL_H_INCLUDED
#define UI_LABEL_H_INCLUDED

namespace survarium {

struct ui_label {

public:
	/* 0x0000 */	pcstr		name;
	/* 0x0004 */	pcstr		label;
}; // struct ui_label

STATIC_SIZE_ASSERT(ui_label, 0x8);

} // namespace survarium

#endif // #ifndef UI_LABEL_H_INCLUDED
