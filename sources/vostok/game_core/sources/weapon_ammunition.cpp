// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

weapon_ammunition::weapon_ammunition( ) :
	inventory_item	( inventory_item::disabled ),
	m_source		( NULL )
{
}

void weapon_ammunition::load( configs::binary_config_value const& cfg )
{
	m_distance         = (float)cfg["distance_coef"];
	m_dispersion       = (float)cfg["dispersion"];
	m_damage           = (float)cfg["k_damage"];
	m_impulse          = (float)cfg["impulse_coef"];
	m_pierce           = (float)cfg["k_arp"];
	m_air_resistance   = (float)cfg["air_resistance"];
	m_buck_shot        = (s16)cfg["buck_shot"];
	m_game_material_id = (s16)cfg["game_material_id"];
	m_tracer           = (bool)cfg["tracer"];
	m_ricochet_angle   = (float)cfg["ricochet_angle"];
	m_ammo_type        = (ammo_type_enum)(u32)cfg["ammo_type"];
	m_muzzle_speed     = (float)cfg["muzzle_speed"];
}

void weapon_ammunition::update_bones_matrices(
	animation::skeleton_ptr const&			user_skeleton,
	float4x4* const							user_matrices,
	const u32								user_matrices_count,
	const u32								current_time_in_ms,
	float4x4&								character_head_transform,
	float4x4&								character_transform,
	animation::animation_player const&		animation_player
)
{ VOSTOK_UNREFERENCED_PARAMETERS( &user_skeleton, user_matrices, user_matrices_count, current_time_in_ms, character_head_transform, character_transform, &animation_player ); }

} // namespace survarium
