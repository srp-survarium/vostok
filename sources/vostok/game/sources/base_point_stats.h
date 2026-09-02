// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef BASE_POINT_STATS_H_INCLUDED
#define BASE_POINT_STATS_H_INCLUDED

// the canonical dump re-prints game_team_id here; the enum lives in game_core
#include <vostok/game_core/game_team_id.h>

namespace survarium {

struct base_point_stats {

public:
	/* 0x0000 */	u32				team_points_amount;
	/* 0x0004 */	u32				capture_progress;
	/* 0x0008 */	game_team_id	point_owner;
}; // struct base_point_stats

STATIC_SIZE_ASSERT(base_point_stats, 0xC);

} // namespace survarium

#endif // #ifndef BASE_POINT_STATS_H_INCLUDED
