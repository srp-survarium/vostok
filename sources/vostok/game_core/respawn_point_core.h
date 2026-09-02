// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef RESPAWN_POINT_CORE_H_INCLUDED
#define RESPAWN_POINT_CORE_H_INCLUDED

#include <vostok/game_core/game_team_id.h>

namespace survarium {

struct respawn_point_core {
public:
			explicit	respawn_point_core	( );

	virtual	void		load				( configs::binary_config_value const& config );

public:
	/* 0x0004 */	u32				point_id;
	/* 0x0008 */	float3			position;
	/* 0x0014 */	float			orientation;
	/* 0x0018 */	u32				point_priority;
	/* 0x001c */	game_team_id	team_owner;
	/* 0x0020 */	bool			selected_for_respawn;
}; // struct respawn_point_core

STATIC_SIZE_ASSERT(respawn_point_core, 0x24);

} // namespace survarium

#endif // #ifndef RESPAWN_POINT_CORE_H_INCLUDED
