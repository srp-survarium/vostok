////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.instant_chamber_a_round
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_chamber_a_round_state_base::weapon_core_chamber_a_round_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	m_animation_timescale = animation_time_scale;
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

// STATE[100%|DONE]
void weapon_core_chamber_a_round_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );
}

// STATE[100%|DONE]
void weapon_core_chamber_a_round_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	m_weapon.instant_chamber_a_round( );
	animation_player_tick_result = true;
}

// STATE[PARTIAL]: single append( m_animation_has_been_ended ) ([+0x135] bool). Matches target shape.
void weapon_core_chamber_a_round_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append( m_animation_has_been_ended );

	// STRUCTURE DIFF[target 0x751c90 | base 0x454180]: target 1 / base 1 stmts
	//   1: 0x009 <0x13> | 0x009 <0x1a> | packet.append( m_animation_has_been_ended );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet<T>::append LTCG inline (target) vs call (base), non-steerable.
}

// STATE[PARTIAL]: single r< bool > into m_animation_has_been_ended ([+0x135]). Matches target shape.
void weapon_core_chamber_a_round_state_base::deserialize( network_core::packet_reader& reader )
{
	m_animation_has_been_ended = reader.r< bool >( );

	// STRUCTURE DIFF[target 0x751c70 | base 0x454110]: target 1 / base 1 stmts
	//   1: 0x009 <0x11> | 0x009 <0x26> | m_animation_has_been_ended = reader.r< bool >( );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet_reader::r<bool> LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
