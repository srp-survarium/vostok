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

// STATE[92%|PARTIAL]: LTCG inline-vs-call of the trivial accessor round_is_chambered().
// STRUCTURE DIFF[target 0x766720 | base 0x44d6b0]: target 6 / base 4 stmts
// 0x011 <0xf> | 0x011 <0x43> | if ( !deserializing( ) && m_weapon.chamber_a_round_on_reload( ) && m_weapon.round_is_chambered( ) )   SIZE
// 0x020 <0x2f> | --          | L31   ONLY target
// .. same ..
// ; aligned 3, size-diffs 1, quantity-diffs 2
// VERDICT: STRUCTURE MATCH (shape ok) - every stmt, member offset and the && short-circuit are byte-exact; the SIZE/ONLY-target rows are the line-marker split of the same if-condition: at +0x43 the target keeps round_is_chambered() out-of-line (`call`, standalone @0x09b360) while our /GL link inlines it (`mov cl,[eax+48Eh]`). Same unsteerable inline-vs-call class as is_aimed()/get_user(). trail: weapon_core_reload_chamber_state_base.md
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
