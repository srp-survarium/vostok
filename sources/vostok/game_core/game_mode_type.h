// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_MODE_TYPE_H_INCLUDED
#define GAME_MODE_TYPE_H_INCLUDED

namespace survarium {

// shared by game_core's match_options and game's game_world_ui; extracted from
// game_net_defines.h when game began including both (canonical:
// headers/survarium/enums/game_mode_type.h)
enum game_mode_type
{
	capture_enemy_base		= 0x00,
	capture_neutral_base	= 0x01,
	gather_victory_items	= 0x02,
	invalid_game_mode		= 0xff,
};

} // namespace survarium

#endif // #ifndef GAME_MODE_TYPE_H_INCLUDED
