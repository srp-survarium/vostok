////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_finish_substate::weapon_core_shotgun_reload_finish_substate(survarium::weapon_core&, const float, vostok::resources::managed_resource_ptr const*, const unsigned int)
weapon_core_shotgun_reload_finish_substate::weapon_core_shotgun_reload_finish_substate(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_shotgun_reload_base_substate(
		weapon, animation_time_scale,
		animations,
		animations_count,
		animation::mixing::play_once_and_remove_at_end,
		10,
		"animation_id",
		"hands_stand_animation_id",
		"hands_crouch_animation_id",
		"hands_jump_animation_id"
	)
{
	// FUNCTION BODY
	// <0x59e1b0>|0x000|+0x05a:'20'	{
	// <0x59e20a>|0x05a|      :'21'	}
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_finish_substate::is_ready_for_transition() const
bool weapon_core_shotgun_reload_finish_substate::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY
	// <0x59dfb7>|0x007|+0x002:'25'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_finish_substate::initialize()
void weapon_core_shotgun_reload_finish_substate::initialize( )
{
	// FUNCTION BODY
	// <0>
	// <0x59e0dd>|0x00d|+0x0c6:'31'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_finish_substate::finalize()
void weapon_core_shotgun_reload_finish_substate::finalize( )
{
	// FUNCTION BODY
	// <0x59e039>|0x009|+0x01a:'36'
	// <0x59e053>|0x023|+0x014:'37'
	// <0x59e067>|0x037|+0x01a:'38'
	// <0>
	// <0x59e081>|0x051|+0x02f:'40'
	// <0>
	// <0x59e0b0>|0x080|+0x00e:'42'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core_shotgun_reload_finish_substate::on_animation_end(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_core_shotgun_reload_finish_substate::on_animation_end( animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0x59dfc9>|0x009|+0x007:'49'
	// <0>
	// <0x59dfd0>|0x010|+0x010:'51'
	// <0x59dfe0>|0x020|+0x00c:'52'
	// <0x59dfec>|0x02c|+0x01b:'53'
	// <0x59e007>|0x047|+0x00c:'54'
	// <0x59e013>|0x053|+0x007:'55'
	// <0>
	// <1>
	// <2>
	// <0x59e01a>|0x05a|+0x002:'59'
	// ******
}

} // namespace survarium
