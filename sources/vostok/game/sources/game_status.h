// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_STATUS_H_INCLUDED
#define GAME_STATUS_H_INCLUDED

namespace survarium {

enum game_status
{
	game_status_inactive				= 0x0,
	game_status_waiting_for_first_player	= 0x1,
	game_status_waiting_for_players		= 0x2,
	game_status_final_countdown			= 0x3,
	game_status_inprocess				= 0x4,
};

} // namespace survarium

#endif // #ifndef GAME_STATUS_H_INCLUDED
