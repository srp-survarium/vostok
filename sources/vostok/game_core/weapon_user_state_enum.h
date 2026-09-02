// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_USER_STATE_ENUM_H_INCLUDED
#define WEAPON_USER_STATE_ENUM_H_INCLUDED

namespace survarium {

enum weapon_user_state_enum {
	type_stand					= 0x0,
	type_crouch					= 0x1,
	type_sprint					= 0x2,
	type_jump					= 0x3,
	type_preview				= 0x4,
	weapon_user_states_count	= 0x5,
};

} // namespace survarium

#endif // #ifndef WEAPON_USER_STATE_ENUM_H_INCLUDED
