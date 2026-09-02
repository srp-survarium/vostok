// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_TEAM_ID_H_INCLUDED
#define GAME_TEAM_ID_H_INCLUDED

namespace survarium {

enum game_team_id {
	team_1			= 0x00,
	team_2			= 0x01,
	team_neutral	= 0x02,
	team_undefined	= 0x03,
	team_invalid	= 0xFF
};

} // namespace survarium

#endif // #ifndef GAME_TEAM_ID_H_INCLUDED
