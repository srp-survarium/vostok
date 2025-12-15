////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_fire_state.h>

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_fire_state::double_barreled_weapon_core_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_fire_state::double_barreled_weapon_core_fire_state(
	weapon_core&						weapon,
	float								animation_time_scale,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count
)
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
	// <0x7ac446><2>
	// <0x7ac45e><3>
	// <0x7ac4ce><3>
	// ******

	// FUNCTION BODY
	// <0x7ac3db>|0x06b|+0x02b:'27'
	// <0x7ac406>|0x096|+0x023:'28'
	// <0x7ac429>|0x0b9|+0x007:'29'
	// <0x7ac430>|0x0c0|+0x018|[1]:'30'
	// <0x7ac448>|0x0d8|+0x018:'31'
	// <0x7ac460>|0x0f0|+0x018:'32'
	// <0x7ac478>|0x108|+0x03a:'33'
	// <0x7ac4b2>|0x142|+0x002:'34'
	// <0x7ac4b4>|0x144|+0x002:'35'
	// <0x7ac4b6>|0x146|+0x002:'36'
	// <0x7ac4b8>|0x148|+0x018|[2]:'37'
	// <0x7ac4d0>|0x160|+0x018:'38'
	// <0x7ac4e8>|0x178|+0x031:'39'
	// <0x7ac519>|0x1a9|+0x002:'40'
	// <0x7ac51b>|0x1ab|+0x002:'41'
	// <0x7ac51d>|0x1ad|+0x00c:'42'
	// ******
}

// STATE[STUB]
// void survarium::double_barreled_weapon_core_fire_state::initialize()
void double_barreled_weapon_core_fire_state::initialize( )
{
	// FUNCTION BODY
	// <0x7ac8c9>|0x009|+0x008:'47'
	// <0x7ac8d1>|0x011|+0x00c:'48'
	// <0x7ac8dd>|0x01d|+0x022:'49'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_fire_state::weapon_and_hands_expression(
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
	// <0x7ac7f0>|0x010|+0x01f:'55'
	// <0x7ac80f>|0x02f|+0x02a:'56'
	// <0x7ac839>|0x059|+0x07a:'57'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7ac719>|0x009|+0x007:'63'
	// <0x7ac720>|0x010|+0x007:'64'
	// <0>
	// <0x7ac727>|0x017|+0x010:'66'
	// <0x7ac737>|0x027|+0x032:'67'
	// <0x7ac769>|0x059|+0x00c:'68'
	// <0>
	// <1>
	// <2>
	// <0x7ac775>|0x065|+0x056:'72'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_fire_state::get_user_hands_expression(
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
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7ac5c1>|0x011|+0x006:'77'
	// <0x7ac5c7>|0x017|+0x010:'78'
	// <0>
	// <0x7ac5d7>|0x027|+0x00c:'80'
	// <0x7ac5e3>|0x033|+0x020:'81'
	// <0x7ac603>|0x053|+0x059:'82'
	// <0x7ac65c>|0x0ac|+0x010:'83'
	// <0>
	// <1>
	// <0x7ac66c>|0x0bc|+0x00e:'86'
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
	// <0x7ac67a>|0x0ca|+0x06e:'98'
	// <0x7ac6e8>|0x138|+0x01c:'99'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_fire_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_fire_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
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
	// <0x7ac549>|0x009|+0x05c:'111'
	// ******
}

} // namespace survarium
