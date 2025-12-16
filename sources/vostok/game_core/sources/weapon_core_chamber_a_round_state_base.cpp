////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_chamber_a_round_state_base::weapon_core_chamber_a_round_state_base(survarium::weapon_core&, const float)
weapon_core_chamber_a_round_state_base::weapon_core_chamber_a_round_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	// FUNCTION BODY
	// <0x761d19>|0x059|+0x00d:'22'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_state_base::initialize()
void weapon_core_chamber_a_round_state_base::initialize( )
{
	// FUNCTION BODY
	// <0x761d37>|0x007|+0x008:'27'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_state_base::on_animation_end_impl(bool&)
void weapon_core_chamber_a_round_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	// FUNCTION BODY
	// <0>
	// <0x761c47>|0x007|+0x00e:'34'
	// <0x761c55>|0x015|+0x006:'35'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_state_base::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_chamber_a_round_state_base::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x761c99>|0x009|+0x013:'40'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_chamber_a_round_state_base::deserialize(vostok::network_core::packet_reader&)
void weapon_core_chamber_a_round_state_base::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x761c79>|0x009|+0x011:'45'
	// ******
}

} // namespace survarium
