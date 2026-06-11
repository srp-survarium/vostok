////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_aimed_state_base.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_chamber_a_round_aimed_state_base::weapon_core_chamber_a_round_aimed_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true ),
	m_animation_timescale				( animation_time_scale )
{
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

// STATE[100%|DONE]
void weapon_core_chamber_a_round_aimed_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );
	m_weapon.instant_aim_start( );
}

// STATE[100%|DONE]
void weapon_core_chamber_a_round_aimed_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );
	m_weapon.instant_aim_end( );
}

// STATE[100%|DONE]
void weapon_core_chamber_a_round_aimed_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	m_weapon.instant_chamber_a_round( );
	animation_player_tick_result = true;
}

// STATE[69.33%|PARTIAL]: paired once the header moved it to the private section (target
// mangling @@EBE); residual is append(bool) kept out-of-line in target vs inlined to the
// temp+append(pcvoid,u32) leaf in base, non-steerable LTCG.
void weapon_core_chamber_a_round_aimed_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append( m_animation_has_been_ended );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x7 | 68 | packet.append( m_animation_has_been_ended );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the append(bool) inline-vs-call wall,
	// non-steerable.
}

// STATE[0.00%|PARTIAL]: paired once the header moved it to the private section (target
// mangling @@EAE) but scores 0.0 (report.json omits the field - the paired-at-0 protobuf
// case): the single statement is wholly inside the r<bool> inline-vs-call wall (target 0x11
// out-of-line call vs base 0x26 inlined read+advance), so no instruction pairs.
void weapon_core_chamber_a_round_aimed_state_base::deserialize( network_core::packet_reader& reader )
{
	m_animation_has_been_ended = reader.r< bool >( );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x15 | 77 | m_animation_has_been_ended = reader.r< bool >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the r<bool> inline-vs-call wall,
	// non-steerable.
}

} // namespace survarium
