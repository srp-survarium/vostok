// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_PROFILE_H_INCLUDED
#define PLAYER_PROFILE_H_INCLUDED

#include <vostok/game_core/game_team_id.h>
#include <vostok/game_core/profile_slot.h>
#include <vostok/game_core/skill_booster.h>
#include <vostok/game_core/profile_slot_enum.h>

namespace survarium {

struct player_profile {
public:
	/* 0x0000 */	u32						account_id;
	/* 0x0004 */	u32						profile_id;
	/* 0x0008 */	char					profile_name[32];
	/* 0x0028 */	skill_booster			boosters[11];
	/* 0x0080 */	profile_slot			slots[max_slots_count];
	/* 0x01b0 */	game_team_id			team;
	/* 0x01b4 */	bool					is_local;

			explicit	player_profile	( ) :
				account_id	( 0 ),
				profile_id	( 0 ),
				team		( team_undefined ),
				is_local	( false )
			{
				profile_name[ 0 ] = 0; memory::zero( boosters );
			}

	inline	void		serialize		( network_core::udp_match_packet& packet ) const { /* no source */ }
			void		deserialize		( network_core::packet_reader& reader );
}; // struct player_profile

STATIC_SIZE_ASSERT(player_profile, 0x1B8);

} // namespace survarium

#endif // #ifndef PLAYER_PROFILE_H_INCLUDED
