////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_input.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
player_input::player_input( ) :
	angular_velocity	( 0.0f, 0.0f ),
	angular_acceleration( 0.0f, 0.0f ),
	actions_mask		( 0 )
{
}

// STATE[61.58%|PARTIAL]: 3 appends (float2, float2, u32) - shape matches target.
void player_input::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( angular_velocity );
	packet.append		( angular_acceleration );
	packet.append		( actions_mask );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// TRGT_ONLY|0x6f0e89|--      |0xb |--  |--  |--
	// SIZE +0x2|0x6f0ea2|0x52e727|0xf |0x11|0   |packet.append		( angular_acceleration );
	// BASE_ONLY|--      |0x52e738|--  |0x17|+1  |packet.append		( actions_mask );
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 appends in identical order (target <0xb>,<0xe>,<0xf>); the ONLY rows are SIZE-drift mis-pairing of the LTCG-inlined packet<T>::append, non-steerable.
}

// STATE[63.13%|PARTIAL]: 3 reads (float2, float2, u32) into members - shape matches target.
void player_input::deserialize( network_core::packet_reader& reader )
{
	angular_velocity		= reader.r< math::float2 >( );
	angular_acceleration	= reader.r< math::float2 >( );
	actions_mask			= reader.r< u32 >( );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0x2|0x6f0dfb|0x52e7ff|0x32|0x34|0   |angular_velocity		= reader.r< math::float2 >( );
	// SIZE +0x2|0x6f0e2d|0x52e833|0x33|0x35|+1  |angular_acceleration	= reader.r< math::float2 >( );
	// SIZE +0xe|0x6f0e60|0x52e868|0xe |0x1c|+2  |actions_mask			= reader.r< u32 >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 reads; SIZE rows are the r<float2>/r<u32> wrapper inlined into the caller on both sides but the inner r() folded by LTCG on the target, non-steerable.
}

// STATE[100%|DONE]
bool player_input::is_empty( ) const
{
	return math::is_zero( angular_velocity.x )
		&& math::is_zero( angular_velocity.y )
		&& math::is_zero( angular_acceleration.x )
		&& math::is_zero( angular_acceleration.y )
		&& !actions_mask
		&& true; // sushi@NOTE: master_gold check

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x700d69>|0x009|+0x07e:'56'
	// ******
}

} // namespace survarium
