////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_INPUT_H_INCLUDED
#define PLAYER_INPUT_H_INCLUDED

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}


namespace survarium {

struct player_input {
public:
					player_input		( );

			void	serialize			( network_core::udp_match_packet& packet ) const;
			void	deserialize			( network_core::packet_reader& reader );

			bool	is_empty			( ) const;
	inline	bool	is_moving			( ) const { /* no source */ }
	// claude@MATCH: body proven by weapon_user_animations_selector::tick target asm
	// ([input+10h] & 0x200 with the !=0 neg/sbb/neg bool materialization). sushi@TODO: mask constant
	inline	bool	is_trying_to_sprint	( ) const { return ( actions_mask & 0x200 ) != 0; }
	inline	bool	is_sprinting		( ) const;
	inline	bool	is_walking			( ) const { /* no source */ }
	inline	bool	is_crouching		( ) const { /* no source */ }
	inline	bool	is_jumping			( ) const { /* no source */ }
	inline	bool	is_using			( ) const { /* no source */ }
	inline	bool	is_aiming			( ) const { /* no source */ }
	inline	bool	is_shooting			( ) const { /* no source */ }
	inline	bool	is_holding_breath	( ) const { /* no source */ }


public:
	/* 0x0000 */	float2		angular_velocity;
	/* 0x0008 */	float2		angular_acceleration;
	/* 0x0010 */	u32			actions_mask;
}; // struct player_input

STATIC_SIZE_ASSERT(player_input, 0x14);

} // namespace survarium

#include <vostok/game_core/player_input_inline.h>

#endif // #ifndef PLAYER_INPUT_H_INCLUDED
