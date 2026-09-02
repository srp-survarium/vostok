// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_TARGETS_H_INCLUDED
#define WEAPON_TARGETS_H_INCLUDED

namespace survarium {

enum weapon_targets {
	weapon_target_idle		= 0x0000,
	weapon_target_fire		= 0x0001,
	weapon_target_aim		= 0x0002,
	weapon_target_aim_fire	= 0x0003,
	weapon_target_reload	= 0x0004,
	weapon_target_inactive	= 0x0005,
	weapon_targets_count	= 0x0006,
};

} // namespace survarium

#endif // #ifndef WEAPON_TARGETS_H_INCLUDED
