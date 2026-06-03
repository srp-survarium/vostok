////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_aimed_fire_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_aimed_fire_state::pistol_weapon_core_aimed_fire_state(survarium::weapon_core&, const float, vostok::resources::managed_resource_ptr const*, const unsigned int)
pistol_weapon_core_aimed_fire_state::pistol_weapon_core_aimed_fire_state(
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
	// <0x7aac6b><2>
	// <0x7aac83><3>
	// <0x7aacf3><3>
	// ******

	// FUNCTION BODY[0x7aabc0]: 15
	// <0x7aac2b>|0x06b|+0x023:'26'
	// <0x7aac4e>|0x08e|+0x007:'27'
	// <0x7aac55>|0x095|+0x018|[1]:'28'
	// <0x7aac6d>|0x0ad|+0x018:'29'
	// <0x7aac85>|0x0c5|+0x018:'30'
	// <0x7aac9d>|0x0dd|+0x03a:'31'
	// <0x7aacd7>|0x117|+0x002:'32'
	// <0x7aacd9>|0x119|+0x002:'33'
	// <0x7aacdb>|0x11b|+0x002:'34'
	// <0x7aacdd>|0x11d|+0x018|[2]:'35'
	// <0x7aacf5>|0x135|+0x018:'36'
	// <0x7aad0d>|0x14d|+0x031:'37'
	// <0x7aad3e>|0x17e|+0x002:'38'
	// <0x7aad40>|0x180|+0x002:'39'
	// <0x7aad42>|0x182|+0x00c:'40'
	// ******
}

// STATE[STUB]
// void survarium::pistol_weapon_core_aimed_fire_state::initialize()
void pistol_weapon_core_aimed_fire_state::initialize( )
{
	// LOCALS
	// bool 						last_shot
	// ******

	// FUNCTION BODY[0x7ab0e0]: 3
	// <0x7ab0e9>|0x009|+0x008:'45'
	// <0x7ab0f1>|0x011|+0x05d:'46'
	// <0x7ab14e>|0x06e|+0x013:'47'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression(
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

	// FUNCTION BODY[0x7ab000]: 3
	// <0x7ab010>|0x010|+0x01f:'52'
	// <0x7ab02f>|0x02f|+0x02a:'53'
	// <0x7ab059>|0x059|+0x07a:'54'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY[0x7aaf30]: 11
	// <0>
	// <0x7aaf39>|0x009|+0x007:'60'
	// <0x7aaf40>|0x010|+0x007:'61'
	// <0>
	// <0x7aaf47>|0x017|+0x010:'63'
	// <0x7aaf57>|0x027|+0x032:'64'
	// <0x7aaf89>|0x059|+0x00c:'65'
	// <0>
	// <1>
	// <2>
	// <0x7aaf95>|0x065|+0x05d:'69'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_aimed_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_aimed_fire_state::get_user_hands_expression(
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
	// resources::managed_resource_ptr const& selected_animation
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY[0x7aadd0]: 23
	// <0x7aade1>|0x011|+0x006:'75'
	// <0x7aade7>|0x017|+0x010:'76'
	// <0>
	// <0x7aadf7>|0x027|+0x00c:'78'
	// <0x7aae03>|0x033|+0x020:'79'
	// <0x7aae23>|0x053|+0x059:'80'
	// <0x7aae7c>|0x0ac|+0x010:'81'
	// <0>
	// <1>
	// <0x7aae8c>|0x0bc|+0x00e:'84'
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
	// <0x7aae9a>|0x0ca|+0x06e:'96'
	// <0x7aaf08>|0x138|+0x01c:'97'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_aimed_fire_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
pistol_weapon_core_aimed_fire_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY[0x7aad60]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7aad69>|0x009|+0x05c:'109'
	// ******
}

} // namespace survarium
