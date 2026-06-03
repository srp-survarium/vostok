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
// Every statement, member offset (m_weapon@0x128, m_chamber_a_round_on_reload@0x48F,
// m_is_round_chambered@0x48E) and the && short-circuit match the target byte-for-byte;
// the sole residual is that the target keeps round_is_chambered() out-of-line
// (target standalone @0x09b360, `call ...round_is_chambered`) while our /GL LTCG inlines
// it (`mov cl,[eax+48Eh]`). chamber_a_round_on_reload() is inlined on BOTH sides (no
// standalone in either), so it matches. Same unsteerable inline-vs-call class as
// is_aimed()/get_user() (assembly_patterns.md). No source change steers it.
void weapon_core_reload_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );

	if ( !deserializing( ) && m_weapon.chamber_a_round_on_reload( ) && m_weapon.round_is_chambered( ) )
		m_weapon.unload_chambered_round( );

	// FUNCTION BODY
	// <0x776729>|0x009|+0x008:'27'		weapon_core_animation_end_aware_state::initialize( );
	// <0>
	// <0x776731>|0x011|+0x00f:'29'		if ( !deserializing( ) && chamber_a_round_on_reload( ) && round_is_chambered( ) )
	// <0>
	// <0x776740>|0x020|+0x02f:'31'		(the && term reads + the unload_chambered_round leaf)
	// <0x77676f>|0x04f|+0x00e:'32'		m_weapon.unload_chambered_round( );
	// <0>
	// ******
	// TARGET @0x43: mov ecx,[ebp-8]; mov eax,[ecx+128h]; call survarium::weapon_core::round_is_chambered
	// BASE   @0x43: mov edx,[ebp-8]; mov eax,[edx+128h]; mov cl,[eax+48Eh]  (inlined accessor)
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
