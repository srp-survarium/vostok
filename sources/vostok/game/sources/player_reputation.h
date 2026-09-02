// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_REPUTATION_H_INCLUDED
#define PLAYER_REPUTATION_H_INCLUDED

namespace survarium {

struct player_reputation {

public:
	/* 0x0000 */	u8		faction_id;
	/* 0x0002 */	u16		reputation_points;
}; // struct player_reputation

STATIC_SIZE_ASSERT(player_reputation, 0x4);

} // namespace survarium

#endif // #ifndef PLAYER_REPUTATION_H_INCLUDED
