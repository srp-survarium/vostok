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

// STATE[PARTIAL]: 3 appends (float2, float2, u32) - shape matches target.
void player_input::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( angular_velocity );
	packet.append		( angular_acceleration );
	packet.append		( actions_mask );

	// STRUCTURE DIFF[target 0x6f0e80 | base 0x51b0b0]: target 3 / base 3 stmts
	//   1: 0x009 <0xb> | --          | L23   ONLY target
	//   3: 0x022 <0xf> | 0x017 <0x11> | packet.append		( angular_acceleration );   SIZE
	//   4: --          | 0x028 <0x17> | packet.append		( actions_mask );   ONLY base
	// ; aligned 1, size-diffs 1, quantity-diffs 2, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 appends in identical order (target <0xb>,<0xe>,<0xf>); ONLY rows are SIZE-drift mis-pairing of the LTCG-inlined packet<T>::append, non-steerable.
}

// STATE[PARTIAL]: 3 reads (float2, float2, u32) into members - shape matches target.
void player_input::deserialize( network_core::packet_reader& reader )
{
	angular_velocity		= reader.r< math::float2 >( );
	angular_acceleration	= reader.r< math::float2 >( );
	actions_mask			= reader.r< u32 >( );

	// STRUCTURE DIFF[target 0x6f0df0 | base 0x51afc0]: target 3 / base 3 stmts
	//   1: 0x00b <0x32> | 0x009 <0x36> | angular_velocity		= reader.r< math::float2 >( );   SIZE
	//   2: 0x03d <0x33> | 0x03f <0x37> | angular_acceleration	= reader.r< math::float2 >( );   SIZE
	//   3: 0x070 <0xe> | 0x076 <0x23> | actions_mask			= reader.r< u32 >( );   SIZE
	// ; aligned 0, size-diffs 3, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 reads; SIZE rows are r<float2>/r<u32> LTCG inline (target) vs call (base), non-steerable.
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
