////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_PROFILE_H_INCLUDED
#define PLAYER_PROFILE_H_INCLUDED

namespace survarium {

#include <vostok/game_core/game_team_id.h>
#include <vostok/game_core/profile_slot.h>
#include <vostok/game_core/skill_booster.h>

struct survarium::profile_slot;
struct survarium::skill_booster;

struct player_profile {
public:
					player_profile	( );

	inline	void	serialize		( network_core::udp_match_packet& packet ) const { /* no source */ }
			void	deserialize		( network_core::packet_reader& reader );

public:
	/* 0x0000 */	u32						account_id;
	/* 0x0004 */	u32						profile_id;
	/* 0x0008 */	char[32]				profile_name;
	/* 0x0028 */	skill_booster[11]		boosters;
	/* 0x0080 */	profile_slot[19]		slots;
	/* 0x01b0 */	game_team_id			team;
	/* 0x01b4 */	bool					is_local;
}; // struct player_profile

STATIC_SIZE_ASSERT(player_profile, 0x1B8);

} // namespace survarium

#endif // #ifndef PLAYER_PROFILE_H_INCLUDED
