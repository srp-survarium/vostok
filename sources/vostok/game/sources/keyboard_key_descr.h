// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KEYBOARD_KEY_DESCR_H_INCLUDED
#define KEYBOARD_KEY_DESCR_H_INCLUDED

namespace survarium {

struct keyboard_key_descr {

public:
	/* 0x0000 */	pcstr		key_name;
	/* 0x0004 */	s32			dik;
	/* 0x0008 */	char		key_local_name[128];
}; // struct keyboard_key_descr

STATIC_SIZE_ASSERT(keyboard_key_descr, 0x88);

} // namespace survarium

#endif // #ifndef KEYBOARD_KEY_DESCR_H_INCLUDED
