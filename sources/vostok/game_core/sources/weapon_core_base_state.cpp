////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.deserializing() inlines a member read

namespace survarium {

// STATE[INPROGRESS]: bytes 100% (report.json fuzzy_match_percent=100.0) but SOURCE STRUCTURE does not
// match. structure-diff: target 0 stmts / base 6 stmts - the target packs every member init onto the
// ctor decl line (member-init list, no body statements), while our base emits the 5 assignments as
// separate body statements (the diff flags 6 ONLY-base quantity divergences: the 5 assigns + 1 EMPTY).
// Fix: move the 5 assignments into the member-initializer list so the body collapses to the target's 0
// statements. Needs a rebuild to confirm. See .md.
weapon_core_base_state::weapon_core_base_state( weapon_core& weapon, bool serialize_animation_state ) : m_weapon( weapon )
{
	m_is_firing_ptr				= NULL;
	m_body_part_mask_for_user	= animation::body_part_whole_body;	// -1
	m_is_ready_to_be_deactivated	= false;
	m_animation_has_been_ended	= false;
	m_serialize_animation_state	= serialize_animation_state;

	// STRUCTURE DIFF[target 0x6ecf90 | base 0x4594c0]: target 0 / base 6 stmts
	// --          | <0>         |    EMPTY only base
	// --          | 0x070 <0xd> | m_is_firing_ptr				= NULL;   ONLY base
	// --          | 0x07d <0xd> | m_body_part_mask_for_user	= animation::body_part_whole_body;	// -1   ONLY base
	// --          | 0x08a <0xa> | m_is_ready_to_be_deactivated	= false;   ONLY base
	// --          | 0x094 <0xa> | m_animation_has_been_ended	= false;   ONLY base
	// --          | 0x09e <0xc> | m_serialize_animation_state	= serialize_animation_state;   ONLY base
	// ; aligned 0, size-diffs 0, quantity-diffs 6
}

// STATE[100%|DONE]
bool weapon_core_base_state::deserializing( ) const
{
	return m_weapon.deserializing( );
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_base_state::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_base_state::serialize( network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// animation::animation_playback_state playback_state<1>
	// base_player const& 			user<1>
	// bool 						success<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fcef5><1>
	// ******

	// CALL SITE INFO
	// <0x6fcf47> -> bool <unknown>(pcvoid const, const u32, animation::animation_playback_state&) const
	// ******

	// FUNCTION BODY
	// <0x6fcee9>|0x009|+0x012:'33'
	// <0>
	// <1>
	// <2>
	// <0x6fcefb>|0x01b|+0x014:'37'
	// <0x6fcf0f>|0x02f|+0x00c:'38'
	// <0x6fcf1b>|0x03b|+0x011:'39'
	// <0>
	// <1>
	// <0x6fcf2c>|0x04c|+0x020:'42'
	// <0x6fcf4c>|0x06c|+0x008:'43'
	// <0>
	// <1>
	// <0x6fcf54>|0x074|+0x007:'46'
	// <0x6fcf5b>|0x07b|+0x00d:'47'
	// <0>
	// <1>
	// <0x6fcf68>|0x088|+0x00c:'50'
	// <0x6fcf74>|0x094|+0x00f:'51'
	// <0>
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_base_state::deserialize(vostok::network_core::packet_reader&)
void weapon_core_base_state::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x6fce99>|0x009|+0x00e:'57'
	// <0>
	// <0x6fcea7>|0x017|+0x011:'59'
	// <0x6fceb8>|0x028|+0x013:'60'
	// <0>
	// ******
}

} // namespace survarium
