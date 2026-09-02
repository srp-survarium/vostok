// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_reload_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.* + chamber_a_round_on_reload/round_is_chambered/unload_chambered_round/instant_reload
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

weapon_core_reload_state_base::weapon_core_reload_state_base( weapon_core& weapon, const float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true ),
	m_animation_timescale( animation_time_scale )
{
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

void weapon_core_reload_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );

	if ( !deserializing( ) )
	{
		if ( m_weapon.chamber_a_round_on_reload( ) && m_weapon.round_is_chambered( ) )
			m_weapon.unload_chambered_round( );
	}
}

void weapon_core_reload_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	m_weapon.instant_reload( );
	animation_player_tick_result = true;
}

// claude@NOTE: target keeps packet<>::append(const bool) / packet_reader::r<bool> OUT of line
// (single push of the bool value / al return); our base inlines both to the append(&v,sizeof)/
// r(&v,1,1) bodies. Inline ceiling owned by the shared packet.h/packet_reader.h templates, not
// steerable from here; every bool-append/r<bool> sibling sits at the same 69.33%/46.9%.
void weapon_core_reload_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append( m_animation_has_been_ended );
}

void weapon_core_reload_state_base::deserialize( network_core::packet_reader& reader )
{
	m_animation_has_been_ended = reader.r< bool >( );
}

} // namespace survarium
