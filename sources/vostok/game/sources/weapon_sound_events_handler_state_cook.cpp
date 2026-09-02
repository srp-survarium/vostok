// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "weapon.h"
#include "weapon_sound_events_handler_state_cook.h"

#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// claude@NOTE: all four calculators STRUCTURE MATCH (single return). The byte residual is
// cross-unit: shotgun's get_magazine_capacity is out-of-line in weapon_core.cpp (target
// inlines the m_magazine_capacity load at weapon+0x478). Faithfully forward-declared -
// the target calls these, it does not include the inline header.
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

#define DEFINE_WEAPON_SOUND_STATE_CALCULATOR( state_type, calculator ) \
	weapon_sound_events_handler_state_cook_traits< weapon_sound_events_handler_state< state_type > >::animation_time_scale_calculator_type \
	weapon_sound_events_handler_state_cook_traits< weapon_sound_events_handler_state< state_type > >::animation_time_scale_calculator = calculator

DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_reload_state, reload_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_chamber_a_round_state, always_unit_timescale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_chamber_a_round_aimed_state, always_unit_timescale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_fire_state, fire_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_aimed_fire_state, fire_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_shotgun_reload_start_substate, always_unit_timescale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_shotgun_reload_one_round_substate, shotgun_reload_timescale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( weapon_core_shotgun_reload_finish_substate, always_unit_timescale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( pistol_weapon_core_reload_state, reload_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( pistol_weapon_core_fire_state, fire_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( pistol_weapon_core_aimed_fire_state, fire_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( double_barreled_weapon_core_reload_state, reload_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( double_barreled_weapon_core_fire_state, fire_animation_time_scale_calculator );
DEFINE_WEAPON_SOUND_STATE_CALCULATOR( double_barreled_weapon_core_aimed_fire_state, fire_animation_time_scale_calculator );

#undef DEFINE_WEAPON_SOUND_STATE_CALCULATOR

} // namespace survarium
