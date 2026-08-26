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
					player_input		( );

			void	serialize			( network_core::udp_match_packet& packet ) const;
			void	deserialize			( network_core::packet_reader& reader );

			bool	is_empty			( ) const;
	inline	bool	is_moving			( ) const { /* no source */ }
	// Target callers materialize the bool result before consuming it.
	__forceinline bool is_trying_to_sprint	( ) const { return ( actions_mask & 0x200 ) != 0; }
	__declspec(noinline) inline bool is_sprinting	( ) const;
	inline	bool	is_walking			( ) const { /* no source */ }
	inline	bool	is_crouching		( ) const { return ( actions_mask & 0x100 ) != 0; }
	inline	bool	is_jumping			( ) const { return ( actions_mask & 0x10 ) != 0; }
	inline	bool	is_using			( ) const { return ( actions_mask & 0x10000000 ) != 0; }
	inline	bool	is_aiming			( ) const { /* no source */ }
	inline	bool	is_shooting			( ) const { /* no source */ }
	inline	bool	is_holding_breath	( ) const { /* no source */ }

	typedef u32 actions_mask_type;

public:
	/* 0x0000 */	float2		angular_velocity;
	/* 0x0008 */	float2		angular_acceleration;
	/* 0x0010 */	u32			actions_mask;
}; // struct player_input

STATIC_SIZE_ASSERT(player_input, 0x14);

} // namespace survarium

#include <vostok/game_core/player_input_inline.h>

#endif // #ifndef PLAYER_INPUT_H_INCLUDED
