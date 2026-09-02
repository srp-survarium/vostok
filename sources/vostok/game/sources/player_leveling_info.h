// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_LEVELING_INFO_H_INCLUDED
#define PLAYER_LEVELING_INFO_H_INCLUDED

namespace survarium {

struct player_leveling_info {

public:
	/* 0x0000 */	u8		total_skill_points;
	/* 0x0004 */	u32		total_experience;
	/* 0x0008 */	u32		prev_level_experience;
	/* 0x000c */	u32		next_level_experience;
}; // struct player_leveling_info

STATIC_SIZE_ASSERT(player_leveling_info, 0x10);

} // namespace survarium

#endif // #ifndef PLAYER_LEVELING_INFO_H_INCLUDED
