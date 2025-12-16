////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_idle_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_idle_state::double_barreled_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::managed_resource_ptr const*, const unsigned int)
double_barreled_weapon_core_idle_state::double_barreled_weapon_core_idle_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count ) :
	weapon_core_idle_state_base( weapon )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// ******

	// FUNCTION BODY
	// <0x7acc6f>|0x03f|+0x00c:'21'
	// <0x7acc7b>|0x04b|+0x023:'22'
	// <0>
	// <0x7acc9e>|0x06e|+0x007:'24'
	// <0x7acca5>|0x075|+0x018|[1]:'25'
	// <0x7accbd>|0x08d|+0x018|[2]:'26'
	// <0x7accd5>|0x0a5|+0x018|[3]:'27'
	// <0x7acced>|0x0bd|+0x03c:'28'
	// <0x7acd29>|0x0f9|+0x002:'29'
	// <0x7acd2b>|0x0fb|+0x002:'30'
	// <0x7acd2d>|0x0fd|+0x005:'31'
	// <0x7acd32>|0x102|+0x00c:'32'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_idle_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7ace91>|0x011|+0x036:'37'
	// <0x7acec7>|0x047|+0x01f:'38'
	// <0x7acee6>|0x066|+0x059:'39'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7acdb9>|0x009|+0x00c:'44'
	// <0>
	// <0x7acdc5>|0x015|+0x007:'46'
	// <0x7acdcc>|0x01c|+0x007:'47'
	// <0x7acdd3>|0x023|+0x007:'48'
	// <0>
	// <0x7acdda>|0x02a|+0x014:'50'
	// <0x7acdee>|0x03e|+0x00a:'51'
	// <0x7acdf8>|0x048|+0x02e:'52'
	// <0x7ace26>|0x076|+0x04f:'53'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_idle_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_idle_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
double_barreled_weapon_core_idle_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_idle_state>::new_object(
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
	// <0x7acd59>|0x009|+0x045:'64'
	// ******
}

} // namespace survarium
