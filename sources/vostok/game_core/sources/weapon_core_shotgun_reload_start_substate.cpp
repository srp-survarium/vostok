////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>

namespace survarium {

// STATE[STUB]
weapon_core_shotgun_reload_start_substate::weapon_core_shotgun_reload_start_substate(
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
	// FUNCTION BODY[0x59e620]: 0
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_start_substate::initialize()
void weapon_core_shotgun_reload_start_substate::initialize( )
{
	// FUNCTION BODY[0x59e4e0]: 9
	// <0>
	// <0x59e4ea>|0x00a|+0x00a:'26'
	// <0x59e4f4>|0x014|+0x0c6:'27'
	// <0>
	// <0x59e5ba>|0x0da|+0x01a:'29'
	// <0>
	// <0x59e5d4>|0x0f4|+0x02f:'31'
	// <0x59e603>|0x123|+0x00e:'32'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_start_substate::finalize()
void weapon_core_shotgun_reload_start_substate::finalize( )
{
	// FUNCTION BODY[0x59e4a0]: 2
	// <0x59e4a9>|0x009|+0x01a:'38'
	// <0x59e4c3>|0x023|+0x014:'39'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_start_substate::is_ready_for_transition() const
bool weapon_core_shotgun_reload_start_substate::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY[0x59e420]: 1
	// <0x59e427>|0x007|+0x009:'44'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core_shotgun_reload_start_substate::on_animation_end(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_core_shotgun_reload_start_substate::on_animation_end( animation::animation_callback_params& params )
{
	// FUNCTION BODY[0x59e440]: 11
	// <0x59e449>|0x009|+0x007:'49'
	// <0>
	// <0x59e450>|0x010|+0x010:'51'
	// <0x59e460>|0x020|+0x00c:'52'
	// <0x59e46c>|0x02c|+0x01b:'53'
	// <0x59e487>|0x047|+0x00a:'54'
	// <0x59e491>|0x051|+0x007:'55'
	// <0>
	// <1>
	// <2>
	// <0x59e498>|0x058|+0x002:'59'
	// ******
}

} // namespace survarium
