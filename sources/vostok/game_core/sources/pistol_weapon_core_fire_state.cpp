////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_fire_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_fire_state::pistol_weapon_core_fire_state(survarium::weapon_core&, const float, vostok::resources::managed_resource_ptr const*, const unsigned int)
pistol_weapon_core_fire_state::pistol_weapon_core_fire_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_fire_state_base( weapon, animation_time_scale )
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
	// <0x7ab21b><2>
	// <0x7ab233><3>
	// <0x7ab2a3><3>
	// ******

	// FUNCTION BODY[0x7ab170]: 15
	// <0x7ab1db>|0x06b|+0x023:'26'
	// <0x7ab1fe>|0x08e|+0x007:'27'
	// <0x7ab205>|0x095|+0x018|[1]:'28'
	// <0x7ab21d>|0x0ad|+0x018:'29'
	// <0x7ab235>|0x0c5|+0x018:'30'
	// <0x7ab24d>|0x0dd|+0x03a:'31'
	// <0x7ab287>|0x117|+0x002:'32'
	// <0x7ab289>|0x119|+0x002:'33'
	// <0x7ab28b>|0x11b|+0x002:'34'
	// <0x7ab28d>|0x11d|+0x018|[2]:'35'
	// <0x7ab2a5>|0x135|+0x018:'36'
	// <0x7ab2bd>|0x14d|+0x031:'37'
	// <0x7ab2ee>|0x17e|+0x002:'38'
	// <0x7ab2f0>|0x180|+0x002:'39'
	// <0x7ab2f2>|0x182|+0x00c:'40'
	// ******
}

// STATE[STUB]
// void survarium::pistol_weapon_core_fire_state::initialize()
void pistol_weapon_core_fire_state::initialize( )
{
	// LOCALS
	// bool 						last_shot
	// ******

	// FUNCTION BODY[0x7ab710]: 3
	// <0x7ab719>|0x009|+0x008:'45'
	// <0x7ab721>|0x011|+0x05d:'46'
	// <0x7ab77e>|0x06e|+0x013:'47'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_fire_state::weapon_and_hands_expression(
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

	// FUNCTION BODY[0x7ab5b0]: 9
	// <0x7ab5c1>|0x011|+0x01f:'52'
	// <0>
	// <0x7ab5e0>|0x030|+0x006:'54'
	// <0x7ab5e6>|0x036|+0x05e:'55'
	// <0>
	// <0x7ab644>|0x094|+0x012:'57'
	// <0>
	// <0x7ab656>|0x0a6|+0x02a:'59'
	// <0x7ab680>|0x0d0|+0x07a:'60'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY[0x7ab4e0]: 11
	// <0>
	// <0x7ab4e9>|0x009|+0x007:'66'
	// <0x7ab4f0>|0x010|+0x007:'67'
	// <0>
	// <0x7ab4f7>|0x017|+0x010:'69'
	// <0x7ab507>|0x027|+0x032:'70'
	// <0x7ab539>|0x059|+0x00c:'71'
	// <0>
	// <1>
	// <2>
	// <0x7ab545>|0x065|+0x05d:'75'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_fire_state::get_user_hands_expression(
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

	// FUNCTION BODY[0x7ab380]: 23
	// <0x7ab391>|0x011|+0x006:'80'
	// <0x7ab397>|0x017|+0x010:'81'
	// <0>
	// <0x7ab3a7>|0x027|+0x00c:'83'
	// <0x7ab3b3>|0x033|+0x020:'84'
	// <0x7ab3d3>|0x053|+0x059:'85'
	// <0x7ab42c>|0x0ac|+0x010:'86'
	// <0>
	// <1>
	// <0x7ab43c>|0x0bc|+0x00e:'89'
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
	// <0x7ab44a>|0x0ca|+0x06e:'101'
	// <0x7ab4b8>|0x138|+0x01c:'102'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_fire_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
pistol_weapon_core_fire_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY[0x7ab310]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7ab319>|0x009|+0x05c:'114'
	// ******
}

} // namespace survarium
