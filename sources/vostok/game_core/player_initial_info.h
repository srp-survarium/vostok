// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_INITIAL_INFO_H_INCLUDED
#define PLAYER_INITIAL_INFO_H_INCLUDED

namespace survarium {

struct player_profile;

struct player_initial_info {
	inline explicit		player_initial_info	( ) :
		profile			( NULL ),
		id				( u8(-1) ),
		game_scene		( NULL ),
		is_demo_player	( false )
	{
	}

public:
	/* 0x0000 */	player_profile*		profile;
	/* 0x0004 */	u8					id;
	/* 0x0008 */	void*				game_scene;
	/* 0x000c */	bool				is_demo_player;
}; // struct player_initial_info

STATIC_SIZE_ASSERT(player_initial_info, 0x10);

} // namespace survarium

#endif // #ifndef PLAYER_INITIAL_INFO_H_INCLUDED
