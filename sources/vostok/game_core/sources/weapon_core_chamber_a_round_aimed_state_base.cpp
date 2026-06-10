////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_aimed_state_base.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_chamber_a_round_aimed_state_base::weapon_core_chamber_a_round_aimed_state_base(survarium::weapon_core&, const float)
weapon_core_chamber_a_round_aimed_state_base::weapon_core_chamber_a_round_aimed_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	// FUNCTION BODY
	// <0x73e419>|0x059|+0x00d:'22'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_aimed_state_base::initialize()
void weapon_core_chamber_a_round_aimed_state_base::initialize( )
{
	// CALL SITE INFO
	// <0x73e45b> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x73e437>|0x007|+0x008:'27'
	// <0x73e43f>|0x00f|+0x01e:'28'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_aimed_state_base::finalize()
void weapon_core_chamber_a_round_aimed_state_base::finalize( )
{
	// CALL SITE INFO
	// <0x73e32b> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x73e307>|0x007|+0x008:'34'
	// <0x73e30f>|0x00f|+0x01e:'35'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_aimed_state_base::on_animation_end_impl(bool&)
void weapon_core_chamber_a_round_aimed_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	// FUNCTION BODY
	// <0>
	// <0x73e347>|0x007|+0x00e:'42'
	// <0x73e355>|0x015|+0x006:'43'
	// ******
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
