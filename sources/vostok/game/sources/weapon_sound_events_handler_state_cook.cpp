////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_sound_events_handler_state_cook.h"

#include <vostok/game_core/weapon_state_creation_params.h>

namespace survarium {

// STATE[STUB]
float reload_animation_time_scale_calculator(
	resources::managed_resource_ptr const&	reload_animation,
	weapon_state_creation_params const&		params
)
{
	return 0.0f;

	// FUNCTION BODY[0x5bd240]: 1
	// <0x5bd240>|0x000|+0x021:'43'
	// ******
}

// STATE[STUB]
float fire_animation_time_scale_calculator(
	resources::managed_resource_ptr const&	reload_animation,
	weapon_state_creation_params const&		params
)
{
	return 0.0f;

	// FUNCTION BODY[0x5bd210]: 1
	// <0x5bd210>|0x000|+0x021:'48'
	// ******
}

// STATE[STUB]
float shotgun_reload_timescale_calculator(
	resources::managed_resource_ptr const&	reload_animation,
	weapon_state_creation_params const&		params
)
{
	return 0.0f;

	// FUNCTION BODY[0x5bd1e0]: 1
	// <0x5bd1e0>|0x000|+0x025:'53'
	// ******
}

// PDB: both params __formal, genuinely unused - left unnamed
// STATE[STUB]
float always_unit_timescale_calculator(
	resources::managed_resource_ptr const&,
	weapon_state_creation_params const&
)
{
	return 0.0f;

	// FUNCTION BODY[0x5bd1d0]: 1
	// <0x5bd1d0>|0x000|+0x002:'58'
	// ******
}

} // namespace survarium
