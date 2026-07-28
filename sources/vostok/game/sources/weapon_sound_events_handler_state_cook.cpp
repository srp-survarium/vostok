////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_sound_events_handler_state_cook.h"

#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// claude@NOTE: all four calculators STRUCTURE MATCH (single return). The byte
// residual is cross-unit: computed_shooting/reload_animation_time_scale are STATE[STUB]
// in game_core (weapon_animations_timescale_inline.h) so the call folds + does no work,
// and shotgun's get_magazine_capacity is out-of-line in weapon_core.cpp (target inlines
// the m_magazine_capacity load at weapon+0x478). These rise once those helpers/accessor
// land; faithfully forward-declared (target calls them, does not include the inline hdr).
float computed_reload_animation_time_scale( resources::managed_resource_ptr const& reload_animation, float reload_time );
float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second );
float computed_shotgun_reload_animation_time_scale( resources::managed_resource_ptr const& reload_one_round_anim, u32 magazine_capacity, float reload_time );

float reload_animation_time_scale_calculator(
	resources::managed_resource_ptr const&	reload_animation,
	weapon_state_creation_params const&		params
)
{
	return computed_reload_animation_time_scale( reload_animation, params.reload_time );
}

float fire_animation_time_scale_calculator(
	resources::managed_resource_ptr const&	fire_animation,
	weapon_state_creation_params const&		params
)
{
	return computed_shooting_animation_time_scale( fire_animation, params.rounds_per_second );
}

float shotgun_reload_timescale_calculator(
	resources::managed_resource_ptr const&	reload_animation,
	weapon_state_creation_params const&		params
)
{
	return computed_shotgun_reload_animation_time_scale( reload_animation, params.weapon.get_magazine_capacity( ), params.reload_time );
}

// PDB: both params __formal, genuinely unused - left unnamed
float always_unit_timescale_calculator(
	resources::managed_resource_ptr const&,
	weapon_state_creation_params const&
)
{
	return 1.0f;
}

} // namespace survarium
