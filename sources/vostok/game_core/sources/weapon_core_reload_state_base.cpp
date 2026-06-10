////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_reload_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.* + chamber_a_round_on_reload/round_is_chambered/unload_chambered_round/instant_reload
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_reload_state_base::weapon_core_reload_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	m_animation_timescale = animation_time_scale;
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

// STATE[92%|INPROGRESS]: source SHAPE matches the target (base call + one flat `if` guarding a
// single statement - control-flow skeleton is identical). Residual is NOT structural: target's 3rd
// `&&` term is an out-of-line `call round_is_chambered` @0x09b360, but our base STILL INLINES it
// (`mov cl,[eax+48Eh]`) - the out-of-lining did not take (round_is_chambered absent from base index;
// see weapon_core.cpp STUB). That inline-vs-call is what produces the target's extra L31 row + the
// `if`-head SIZE diff below; the source shape needs no change. Matcher: make round_is_chambered emit
// out-of-line (linker/inline concern, byte not structure). trail: weapon_core_reload_chamber_state_base.md
// STRUCTURE DIFF[target 0x766720 | base 0x44d690]: target 4 / base 3 stmts
// 2: 0x011 <0xf> | 0x011 <0x43> | if ( !deserializing( ) && m_weapon.chamber_a_round_on_reload( ) && m_weapon.round_is_chambered( ) )   SIZE
// 3: 0x020 <0x2f> | --          | L31   ONLY target
// .. same ..
// ; aligned 2, size-diffs 1, quantity-diffs 1, blank-gaps 1
// VERDICT: STRUCTURE MATCH (shape ok) - SIZE+L31 are round_is_chambered inlined(base) vs out-of-line call(target), not a source-shape diff. trail: weapon_core_reload_chamber_state_base.md
void weapon_core_reload_state_base::initialize( )
{
	// sushi@TODO: the structure is wrong here - the verifier's "STRUCTURE MATCH (shape ok)" verdict
	// is disputed (the L31 ONLY-target row is more than just round_is_chambered inline-vs-call). To
	// be dealt with later.
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

// STATE[69.33%|PARTIAL]: single append( m_animation_has_been_ended ) ([+0x135] bool). Matches target shape.
void weapon_core_reload_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append( m_animation_has_been_ended );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ---------+--------+--------+----+----+------+------
	// SIZE +0x7|0x766689|0x467269|0x13|0x1a|55    |	packet.append( m_animation_has_been_ended );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet<T>::append LTCG inline (base) vs call (target), non-steerable.
}

// STATE[46.91%|PARTIAL]: single r< bool > into m_animation_has_been_ended ([+0x135]). Matches target shape.
void weapon_core_reload_state_base::deserialize( network_core::packet_reader& reader )
{
	m_animation_has_been_ended = reader.r< bool >( );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ---------+--------+--------+----+----+------+------
	// SIZE +0xb|0x766669|0x467239|0x11|0x1c|66    |	m_animation_has_been_ended = reader.r< bool >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet_reader::r<bool> LTCG inline (base) vs call (target), non-steerable.
}

} // namespace survarium
