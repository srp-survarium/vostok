// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAIN_MENU_BUTTON_NAME_TO_ACTION_H_INCLUDED
#define MAIN_MENU_BUTTON_NAME_TO_ACTION_H_INCLUDED

namespace survarium {

// aggregate: brace-initialized ( { "name", "action" } ) in game_options.cpp
struct main_menu_button_name_to_action {
public:
	/* 0x0000 */	fixed_string< 32 >		name;
	/* 0x002c */	fixed_string< 32 >		action;
}; // struct main_menu_button_name_to_action

STATIC_SIZE_ASSERT(main_menu_button_name_to_action, 0x58);

} // namespace survarium

#endif // #ifndef MAIN_MENU_BUTTON_NAME_TO_ACTION_H_INCLUDED
