// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/player_input.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

player_input::player_input( ) :
	angular_velocity	( 0.0f, 0.0f ),
	angular_acceleration( 0.0f, 0.0f ),
	actions_mask		( 0 )
{
}

void player_input::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( angular_velocity );
	packet.append		( angular_acceleration );
	packet.append		( actions_mask );
}

// claude@NOTE: STRUCTURE MATCH (3 stmts, lines 30-32). Byte residual is the
// packet_reader::r<T> __declspec(noinline): the /Ox target INLINES r<math::float2>
// (no standalone r<float2> in the target index; the read expands to float2 ctor +
// non-template r(void*,u32,u32) + Vector2::operator= + member store), while our /Od
// base out-of-lines the whole r<float2> call. Header is owned by another unit;
// the noinline is a net win for ~25 handlers whose targets DO out-of-line r<T>.
// Manual-inlining the read here (tested) drops to 57.7% AND breaks the 3-stmt
// structure (7 base stmts). Faithful form kept.
void player_input::deserialize( network_core::packet_reader& reader )
{
	angular_velocity		= reader.r< math::float2 >( );
	angular_acceleration	= reader.r< math::float2 >( );
	actions_mask			= reader.r< u32 >( );
}

bool player_input::is_empty( ) const
{
	return math::is_zero( angular_velocity.x )
		&& math::is_zero( angular_velocity.y )
		&& math::is_zero( angular_acceleration.x )
		&& math::is_zero( angular_acceleration.y )
		&& !actions_mask
		&& true; // sushi@NOTE: master_gold check
}

} // namespace survarium
