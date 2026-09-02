// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CHAT_TAB_H_INCLUDED
#define CHAT_TAB_H_INCLUDED

namespace survarium {

struct chat_tab {

public:
	/* 0x0000 */	pcstr		name;
	/* 0x0004 */	pcstr		color;
	/* 0x0008 */	u8			id;
	/* 0x000c */	pcstr		key;
}; // struct chat_tab

STATIC_SIZE_ASSERT(chat_tab, 0x10);

} // namespace survarium

#endif // #ifndef CHAT_TAB_H_INCLUDED
