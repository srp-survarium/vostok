////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_reload_state_base.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_reload_state_base::weapon_core_reload_state_base(survarium::weapon_core&, const float)
weapon_core_reload_state_base::weapon_core_reload_state_base( weapon_core& weapon, float animation_time_scale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;

	// FUNCTION BODY
	// <0x776709>|0x059|+0x00d:'22'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_reload_state_base::initialize()
void weapon_core_reload_state_base::initialize( )
{
	// FUNCTION BODY
	// <0x776729>|0x009|+0x008:'27'
	// <0>
	// <0x776731>|0x011|+0x00f:'29'
	// <0>
	// <0x776740>|0x020|+0x02f:'31'
	// <0x77676f>|0x04f|+0x00e:'32'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_reload_state_base::on_animation_end_impl(bool&)
void weapon_core_reload_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	// FUNCTION BODY
	// <0>
	// <0x776797>|0x007|+0x00e:'39'
	// <0x7767a5>|0x015|+0x006:'40'
	// ******
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
