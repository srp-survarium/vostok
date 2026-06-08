////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_reload_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.* + chamber_a_round_on_reload/round_is_chambered/unload_chambered_round/instant_reload

namespace survarium {

// STATE[100%|DONE]
weapon_core_reload_state_base::weapon_core_reload_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	m_animation_timescale = animation_time_scale;
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

// STATE[99.83%|DONE]: round_is_chambered() out-lined (decl weapon_core.h, body weapon_core.cpp)
// so the base emits the target's `call round_is_chambered` instead of inlining `m_is_round_chambered`
// (was 92%; reviewer fix). Sole residual: the call's `this` load is `mov eax,[edx+128h]` (target,
// round_is_chambered @0x09b360 takes `this` in eax via the link-time custom calling convention) vs
// `mov ecx,[edx+128h]` (base) - the permitted call-boundary arg-passing class (same as #153's
// ammo_in_magazine). trail: weapon_core_reload_chamber_state_base.md
void weapon_core_reload_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );

	if ( !deserializing( ) && m_weapon.chamber_a_round_on_reload( ) && m_weapon.round_is_chambered( ) )
		m_weapon.unload_chambered_round( );
}

// STATE[100%|DONE]
void weapon_core_reload_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	m_weapon.instant_reload( );
	animation_player_tick_result = true;
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_reload_state_base::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_reload_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x776689>|0x009|+0x013:'45'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_reload_state_base::deserialize(vostok::network_core::packet_reader&)
void weapon_core_reload_state_base::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x776669>|0x009|+0x011:'50'
	// ******
}

} // namespace survarium
