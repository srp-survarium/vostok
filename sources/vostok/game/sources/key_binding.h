// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef KEY_BINDING_H_INCLUDED
#define KEY_BINDING_H_INCLUDED

namespace survarium {

struct game_action_descr;
struct keyboard_key_descr;

struct key_binding {

public:
	/* 0x0000 */	game_action_descr*		m_action;
	/* 0x0004 */	keyboard_key_descr*		m_keyboard[2];
}; // struct key_binding

STATIC_SIZE_ASSERT(key_binding, 0xC);

} // namespace survarium

#endif // #ifndef KEY_BINDING_H_INCLUDED
