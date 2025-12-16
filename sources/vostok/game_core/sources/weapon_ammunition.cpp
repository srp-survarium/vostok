////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_ammunition::weapon_ammunition()
weapon_ammunition::weapon_ammunition( ) : inventory_item( inventory_item::disabled )
{
	// FUNCTION BODY
	// <0x6f9bf0>|0x000|      :'16'	{
	// ******
}

// STATE[STUB]
// void survarium::weapon_ammunition::load(vostok::configs::binary_config_value const&)
void weapon_ammunition::load( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
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

// STATE[STUB]
// void survarium::weapon_ammunition::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void weapon_ammunition::update_bones_matrices(
	resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const&	user_skeleton,
	float4x4*							user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	float4x4&							character_head_transform,
	float4x4&							character_transform,
	animation::animation_player const&	animation_player
)
{
	// FUNCTION BODY
	// <0x6f9a00>|0x000|      :'43'	{
	// ******
}

} // namespace survarium
