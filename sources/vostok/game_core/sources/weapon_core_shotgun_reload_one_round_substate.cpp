////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_one_round_substate::weapon_core_shotgun_reload_one_round_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_shotgun_reload_one_round_substate::weapon_core_shotgun_reload_one_round_substate(
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
	// <0x59e270>|0x000|+0x04d:'19'	{
	// <0x59e2bd>|0x04d|      :'20'	}
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_one_round_substate::is_ready_for_transition() const
bool weapon_core_shotgun_reload_one_round_substate::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY
	// <0x59e227>|0x007|+0x002:'24'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_one_round_substate::initialize()
void weapon_core_shotgun_reload_one_round_substate::initialize( )
{
	// FUNCTION BODY
	// <0>
	// <0x59e34d>|0x00d|+0x0c6:'31'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_one_round_substate::finalize()
void weapon_core_shotgun_reload_one_round_substate::finalize( )
{
	// FUNCTION BODY
	// <0x59e239>|0x009|+0x01a:'36'
	// <0x59e253>|0x023|+0x014:'37'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core_shotgun_reload_one_round_substate::on_animation_end(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_core_shotgun_reload_one_round_substate::on_animation_end( animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0x59e2d9>|0x009|+0x007:'42'
	// <0>
	// <0x59e2e0>|0x010|+0x010:'44'
	// <0x59e2f0>|0x020|+0x00c:'45'
	// <0x59e2fc>|0x02c|+0x01b:'46'
	// <0x59e317>|0x047|+0x00e:'47'
	// <0x59e325>|0x055|+0x007:'48'
	// <0>
	// <1>
	// <0x59e32c>|0x05c|+0x002:'51'
	// ******
}

} // namespace survarium
