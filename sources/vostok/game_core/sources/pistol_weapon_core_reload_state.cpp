////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_reload_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_reload_state::pistol_weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::managed_resource_ptr const*, const unsigned int)
pistol_weapon_core_reload_state::pistol_weapon_core_reload_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_reload_state_base( weapon, animation_timescale )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// u32 							view_index<2>
	// u32 							user_state_index<3>
	// u32 							weapon_state_index<4>
	// ******

	// SKIPPED BLOCKS
	// <0x7a8a4e><2>
	// <0x7a8a66><3>
	// <0x7a8ad6><3>
	// <0x7a8aee><4>
	// ******

	// FUNCTION BODY
	// <0x7a8a0e>|0x05e|+0x023:'24'
	// <0x7a8a31>|0x081|+0x007:'25'
	// <0x7a8a38>|0x088|+0x018|[1]:'26'
	// <0x7a8a50>|0x0a0|+0x018:'27'
	// <0x7a8a68>|0x0b8|+0x018:'28'
	// <0x7a8a80>|0x0d0|+0x03a:'29'
	// <0x7a8aba>|0x10a|+0x002:'30'
	// <0x7a8abc>|0x10c|+0x002:'31'
	// <0x7a8abe>|0x10e|+0x002:'32'
	// <0x7a8ac0>|0x110|+0x018|[2]:'33'
	// <0x7a8ad8>|0x128|+0x018:'34'
	// <0x7a8af0>|0x140|+0x018:'35'
	// <0x7a8b08>|0x158|+0x03a:'36'
	// <0x7a8b42>|0x192|+0x002:'37'
	// <0x7a8b44>|0x194|+0x002:'38'
	// <0x7a8b46>|0x196|+0x002:'39'
	// <0x7a8b48>|0x198|+0x00c:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7a8dd1>|0x011|+0x01f:'45'
	// <0>
	// <0x7a8df0>|0x030|+0x006:'47'
	// <0x7a8df6>|0x036|+0x05e:'48'
	// <0>
	// <0x7a8e54>|0x094|+0x012:'50'
	// <0>
	// <0x7a8e66>|0x0a6|+0x02a:'52'
	// <0>
	// <0x7a8e90>|0x0d0|+0x07a:'54'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_reload_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// u32 							weapon_state_index
	// pcstr[2] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7a8cf9>|0x009|+0x007:'60'
	// <0x7a8d00>|0x010|+0x007:'61'
	// <0>
	// <1>
	// <0x7a8d07>|0x017|+0x01b:'64'
	// <0x7a8d22>|0x032|+0x00a:'65'
	// <0x7a8d2c>|0x03c|+0x02c:'66'
	// <0x7a8d58>|0x068|+0x00c:'67'
	// <0>
	// <1>
	// <2>
	// <0x7a8d64>|0x074|+0x056:'71'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_reload_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_reload_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// u32 							weapon_state_index
	// animation::linear_interpolator interpolator
	// u32 							user_state_index
	// pcstr[2][2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7a8be1>|0x011|+0x00e:'77'
	// <0x7a8bef>|0x01f|+0x00e:'78'
	// <0>
	// <1>
	// <0x7a8bfd>|0x02d|+0x00c:'81'
	// <0x7a8c09>|0x039|+0x01e:'82'
	// <0>
	// <0x7a8c27>|0x057|+0x010:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x7a8c37>|0x067|+0x082:'96'
	// <0x7a8cb9>|0x0e9|+0x021:'97'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_reload_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_reload_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
pistol_weapon_core_reload_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_reload_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7a8b69>|0x009|+0x05c:'109'
	// ******
}

} // namespace survarium
