////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.instant_chamber_a_round

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

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_chamber_a_round_state_base::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_chamber_a_round_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x761c99>|0x009|+0x013:'40'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_chamber_a_round_state_base::deserialize(vostok::network_core::packet_reader&)
void weapon_core_chamber_a_round_state_base::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x761c79>|0x009|+0x011:'45'
	// ******
}

} // namespace survarium
