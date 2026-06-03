////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_chamber_a_round_state::weapon_core_chamber_a_round_state(survarium::weapon_core&, const float, vostok::resources::managed_resource_ptr const*, const unsigned int)
weapon_core_chamber_a_round_state::weapon_core_chamber_a_round_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_chamber_a_round_state_base( weapon, animation_time_scale )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// u32 							view<2>
	// u32 							user_state<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7aa3be><2>
	// <0x7aa423><3>
	// ******

	// FUNCTION BODY[0x7aa320]: 13
	// <0x7aa37e>|0x05e|+0x023:'28'
	// <0x7aa3a1>|0x081|+0x007:'29'
	// <0x7aa3a8>|0x088|+0x018|[1]:'30'
	// <0x7aa3c0>|0x0a0|+0x018:'31'
	// <0x7aa3d8>|0x0b8|+0x031:'32'
	// <0x7aa409>|0x0e9|+0x002:'33'
	// <0x7aa40b>|0x0eb|+0x002:'34'
	// <0x7aa40d>|0x0ed|+0x018|[2]:'35'
	// <0x7aa425>|0x105|+0x018:'36'
	// <0x7aa43d>|0x11d|+0x031:'37'
	// <0x7aa46e>|0x14e|+0x002:'38'
	// <0x7aa470>|0x150|+0x002:'39'
	// <0x7aa472>|0x152|+0x00c:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_chamber_a_round_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_chamber_a_round_state::weapon_and_hands_expression(
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

	// FUNCTION BODY[0x7aa690]: 3
	// <0x7aa6a0>|0x010|+0x01f:'45'
	// <0x7aa6bf>|0x02f|+0x02a:'46'
	// <0x7aa6e9>|0x059|+0x07a:'47'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_chamber_a_round_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_chamber_a_round_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY[0x7aa5f0]: 7
	// <0x7aa5f9>|0x009|+0x007:'52'
	// <0x7aa600>|0x010|+0x023:'53'
	// <0x7aa623>|0x033|+0x00c:'54'
	// <0>
	// <1>
	// <2>
	// <0x7aa62f>|0x03f|+0x056:'58'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_chamber_a_round_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_chamber_a_round_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// pcstr[3] 					animation_captions
	// u32 							user_animation_index
	// ******

	// FUNCTION BODY[0x7aa4f0]: 22
	// <0x7aa501>|0x011|+0x006:'63'
	// <0x7aa507>|0x017|+0x010:'64'
	// <0>
	// <0x7aa517>|0x027|+0x012:'66'
	// <0>
	// <0x7aa529>|0x039|+0x00c:'68'
	// <0>
	// <0x7aa535>|0x045|+0x015:'70'
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
	// <11>
	// <0x7aa54a>|0x05a|+0x079:'83'
	// <0x7aa5c3>|0x0d3|+0x01c:'84'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_chamber_a_round_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_chamber_a_round_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
weapon_core_chamber_a_round_state* weapon_core_state_cook_template<survarium::weapon_core_chamber_a_round_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY[0x7aa490]: 7
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7aa499>|0x009|+0x04b:'97'
	// ******
}

} // namespace survarium
