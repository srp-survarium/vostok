////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_ammunition.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_ammunition::weapon_ammunition()
weapon_ammunition::weapon_ammunition( )
{
}

// STATE[STUB]
// void survarium::weapon_ammunition::load(vostok::configs::binary_config_value const&)
void weapon_ammunition::load( vostok::configs::binary_config_value const& cfg )
{
	// FUNCTION BODY
	// <0x6f9a79>|0x000|0x000:'20'
	// <0x6f9a96>|0x01d|0x01d:'21'
	// <0x6f9ab3>|0x03a|0x01d:'22'
	// <0x6f9ad0>|0x057|0x01d:'23'
	// <0x6f9aed>|0x074|0x01d:'24'
	// <0x6f9b0a>|0x091|0x01d:'25'
	// <0x6f9b27>|0x0ae|0x01d:'26'
	// <0x6f9b49>|0x0d0|0x022:'27'
	// <0x6f9b6b>|0x0f2|0x022:'28'
	// <0x6f9b88>|0x10f|0x01d:'29'
	// <0x6f9ba5>|0x12c|0x01d:'30'
	// <0x6f9bc0>|0x147|0x01b:'31'
	// ******
}

// STATE[STUB]
// void survarium::weapon_ammunition::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void weapon_ammunition::update_bones_matrices(
	vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&	user_skeleton,
	vostok::math::float4x4*				user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	vostok::math::float4x4&				character_head_transform,
	vostok::math::float4x4&				character_transform,
	vostok::animation::animation_player const&	animation_player)
{
}

} // namespace survarium
