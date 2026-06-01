////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_aimed_state_base.h>

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

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_chamber_a_round_aimed_state_base::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_chamber_a_round_aimed_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x73e399>|0x009|+0x013:'48'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_chamber_a_round_aimed_state_base::deserialize(vostok::network_core::packet_reader&)
void weapon_core_chamber_a_round_aimed_state_base::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x73e379>|0x009|+0x011:'53'
	// ******
}

} // namespace survarium
