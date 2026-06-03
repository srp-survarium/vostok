////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

// STATE[100%|DONE]
weapon_ammunition::weapon_ammunition( ) :
	inventory_item	( inventory_item::disabled ),
	m_source		( NULL )
{
	// FUNCTION BODY[0x6f9bf0]
	// ******
}

// STATE[99.29%|DONE]
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


	// FUNCTION BODY[0x6f9a70]: 12
	// <0x6f9a79>|0x009|+0x01d:'20'
	// <0x6f9a96>|0x026|+0x01d:'21'
	// <0x6f9ab3>|0x043|+0x01d:'22'
	// <0x6f9ad0>|0x060|+0x01d:'23'
	// <0x6f9aed>|0x07d|+0x01d:'24'
	// <0x6f9b0a>|0x09a|+0x01d:'25'
	// <0x6f9b27>|0x0b7|+0x022:'26'
	// <0x6f9b49>|0x0d9|+0x022:'27'
	// <0x6f9b6b>|0x0fb|+0x01d:'28'
	// <0x6f9b88>|0x118|+0x01d:'29'
	// <0x6f9ba5>|0x135|+0x01b:'30'
	// <0x6f9bc0>|0x150|+0x01d:'31'
	// ******
}

// STATE[100%|DONE]
void weapon_ammunition::update_bones_matrices(
	animation::skeleton_ptr const&			user_skeleton,
	float4x4* const							user_matrices,
	const u32								user_matrices_count,
	const u32								current_time_in_ms,
	float4x4&								character_head_transform,
	float4x4&								character_transform,
	animation::animation_player const&		animation_player
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( &user_skeleton, user_matrices, user_matrices_count, current_time_in_ms, character_head_transform, character_transform, &animation_player );

	// FUNCTION BODY[0x6f9a00]
	// ******
}

} // namespace survarium
