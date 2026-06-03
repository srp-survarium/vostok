////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_aimed_fire_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
double_barreled_weapon_core_aimed_fire_state::double_barreled_weapon_core_aimed_fire_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_aimed_fire_state_base( weapon, animation_time_scale )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// u32 							view_index<2>
	// u32 							user_state_index<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7abe76><2>
	// <0x7abe8e><3>
	// <0x7abefe><3>
	// ******

	// FUNCTION BODY[0x7abda0]: 16
	// <0x7abe0b>|0x06b|+0x02b:'27'
	// <0x7abe36>|0x096|+0x023:'28'
	// <0x7abe59>|0x0b9|+0x007:'29'
	// <0x7abe60>|0x0c0|+0x018|[1]:'30'
	// <0x7abe78>|0x0d8|+0x018:'31'
	// <0x7abe90>|0x0f0|+0x018:'32'
	// <0x7abea8>|0x108|+0x03a:'33'
	// <0x7abee2>|0x142|+0x002:'34'
	// <0x7abee4>|0x144|+0x002:'35'
	// <0x7abee6>|0x146|+0x002:'36'
	// <0x7abee8>|0x148|+0x018|[2]:'37'
	// <0x7abf00>|0x160|+0x018:'38'
	// <0x7abf18>|0x178|+0x031:'39'
	// <0x7abf49>|0x1a9|+0x002:'40'
	// <0x7abf4b>|0x1ab|+0x002:'41'
	// <0x7abf4d>|0x1ad|+0x00c:'42'
	// ******
}

// STATE[STUB]
// void survarium::double_barreled_weapon_core_aimed_fire_state::initialize()
void double_barreled_weapon_core_aimed_fire_state::initialize( )
{
	// FUNCTION BODY[0x7ac320]: 3
	// <0x7ac329>|0x009|+0x008:'47'
	// <0x7ac331>|0x011|+0x00c:'48'
	// <0x7ac33d>|0x01d|+0x022:'49'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_aimed_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_aimed_fire_state::weapon_and_hands_expression(
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

	// FUNCTION BODY[0x7ac230]: 5
	// <0x7ac240>|0x010|+0x012:'54'
	// <0x7ac252>|0x022|+0x01f:'55'
	// <0x7ac271>|0x041|+0x02a:'56'
	// <0>
	// <0x7ac29b>|0x06b|+0x07a:'58'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_aimed_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY[0x7ac160]: 11
	// <0>
	// <0x7ac169>|0x009|+0x007:'64'
	// <0x7ac170>|0x010|+0x007:'65'
	// <0>
	// <0x7ac177>|0x017|+0x010:'67'
	// <0x7ac187>|0x027|+0x032:'68'
	// <0x7ac1b9>|0x059|+0x00c:'69'
	// <0>
	// <1>
	// <2>
	// <0x7ac1c5>|0x065|+0x056:'73'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_aimed_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_aimed_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// u32 							user_animation_index
	// animation::linear_interpolator interpolator
	// resources::managed_resource_ptr const& selected_animation
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY[0x7abfe0]: 25
	// <0x7abff1>|0x011|+0x006:'78'
	// <0x7abff7>|0x017|+0x010:'79'
	// <0>
	// <0x7ac007>|0x027|+0x00c:'81'
	// <0x7ac013>|0x033|+0x020:'82'
	// <0x7ac033>|0x053|+0x059:'83'
	// <0x7ac08c>|0x0ac|+0x010:'84'
	// <0>
	// <1>
	// <0x7ac09c>|0x0bc|+0x00e:'87'
	// <0>
	// <0x7ac0aa>|0x0ca|+0x010:'89'
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
	// <0x7ac0ba>|0x0da|+0x06e:'101'
	// <0x7ac128>|0x148|+0x021:'102'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_aimed_fire_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
double_barreled_weapon_core_aimed_fire_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY[0x7abf70]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7abf79>|0x009|+0x05c:'114'
	// ******
}

} // namespace survarium
