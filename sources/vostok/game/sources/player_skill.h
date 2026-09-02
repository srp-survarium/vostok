// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_SKILL_H_INCLUDED
#define PLAYER_SKILL_H_INCLUDED

namespace survarium {

struct player_skill {

public:
	/* 0x0000 */	u8		skill_id;
	/* 0x0001 */	u8		skill_points;
}; // struct player_skill

STATIC_SIZE_ASSERT(player_skill, 0x2);

} // namespace survarium

#endif // #ifndef PLAYER_SKILL_H_INCLUDED
