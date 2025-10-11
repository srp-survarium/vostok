////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_shotgun_reload_finish_substate.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_finish_substate::weapon_core_shotgun_reload_finish_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_shotgun_reload_finish_substate::weapon_core_shotgun_reload_finish_substate(
	weapon_core&						weapon,
	float								animation_time_scale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
{
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_finish_substate::is_ready_for_transition() const
bool weapon_core_shotgun_reload_finish_substate::is_ready_for_transition( ) const
{
	return false;
	// FUNCTION BODY
	// <0x59dfb7>|0x000|0x000:'25'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_finish_substate::initialize()
void weapon_core_shotgun_reload_finish_substate::initialize( )
{
	// FUNCTION BODY
	// 1
	// <0x59e0dd>|0x000|0x000:'31'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_finish_substate::finalize()
void weapon_core_shotgun_reload_finish_substate::finalize( )
{
	// FUNCTION BODY
	// <0x59e039>|0x000|0x000:'36'
	// <0x59e053>|0x01a|0x01a:'37'
	// <0x59e067>|0x02e|0x014:'38'
	// 1
	// <0x59e081>|0x048|0x01a:'40'
	// 1
	// <0x59e0b0>|0x077|0x02f:'42'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core_shotgun_reload_finish_substate::on_animation_end(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum weapon_core_shotgun_reload_finish_substate::on_animation_end( vostok::animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0x59dfc9>|0x000|0x000:'49'
	// 1
	// <0x59dfd0>|0x007|0x007:'51'
	// <0x59dfe0>|0x017|0x010:'52'
	// <0x59dfec>|0x023|0x00c:'53'
	// <0x59e007>|0x03e|0x01b:'54'
	// <0x59e013>|0x04a|0x00c:'55'
	// 1
	// 2
	// 3
	// <0x59e01a>|0x051|0x007:'59'
	// ******
}

} // namespace survarium
