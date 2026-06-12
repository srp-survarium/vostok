////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_POINT_STATS_H_INCLUDED
#define BASE_POINT_STATS_H_INCLUDED

namespace survarium {

enum survarium::game_team_id
{
	team_1				= 0x00,
	team_2				= 0x01,
	team_neutral		= 0x02,
	team_undefined		= 0x03,
	team_invalid		= 0xff,
};

struct base_point_stats {

public:
	/* 0x0000 */	u32				team_points_amount;
	/* 0x0004 */	u32				capture_progress;
	/* 0x0008 */	game_team_id	point_owner;
}; // struct base_point_stats

STATIC_SIZE_ASSERT(base_point_stats, 0xC);

} // namespace survarium

#endif // #ifndef BASE_POINT_STATS_H_INCLUDED
