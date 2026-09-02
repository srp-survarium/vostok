// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.instant_chamber_a_round
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

weapon_core_chamber_a_round_state_base::weapon_core_chamber_a_round_state_base( weapon_core& weapon, const float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true ),
	m_animation_timescale( animation_time_scale )
{
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

void weapon_core_chamber_a_round_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );
}

void weapon_core_chamber_a_round_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	m_weapon.instant_chamber_a_round( );
	animation_player_tick_result = true;
}

void weapon_core_chamber_a_round_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append( m_animation_has_been_ended );
}

void weapon_core_chamber_a_round_state_base::deserialize( network_core::packet_reader& reader )
{
	m_animation_has_been_ended = reader.r< bool >( );
}

} // namespace survarium
