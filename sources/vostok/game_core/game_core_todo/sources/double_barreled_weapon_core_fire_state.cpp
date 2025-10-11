////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "double_barreled_weapon_core_fire_state.h"

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_fire_state::double_barreled_weapon_core_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_fire_state::double_barreled_weapon_core_fire_state(
	weapon_core&						weapon,
	float								animation_time_scale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
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
	// <0x7ac3db>|0x000|0x000:'27'
	// <0x7ac406>|0x02b|0x02b:'28'
	// <0x7ac429>|0x04e|0x023:'29'
	// <0x7ac430>|0x055|0x007|[1]:'30'
	// <0x7ac448>|0x06d|0x018:'31'
	// <0x7ac460>|0x085|0x018:'32'
	// <0x7ac478>|0x09d|0x018:'33'
	// <0x7ac4b2>|0x0d7|0x03a:'34'
	// <0x7ac4b4>|0x0d9|0x002:'35'
	// <0x7ac4b6>|0x0db|0x002:'36'
	// <0x7ac4b8>|0x0dd|0x002|[2]:'37'
	// <0x7ac4d0>|0x0f5|0x018:'38'
	// <0x7ac4e8>|0x10d|0x018:'39'
	// <0x7ac519>|0x13e|0x031:'40'
	// <0x7ac51b>|0x140|0x002:'41'
	// <0x7ac51d>|0x142|0x002:'42'
	// ******
}

// STATE[STUB]
// void survarium::double_barreled_weapon_core_fire_state::initialize()
void double_barreled_weapon_core_fire_state::initialize( )
{
	// FUNCTION BODY
	// <0x7ac8c9>|0x000|0x000:'47'
	// <0x7ac8d1>|0x008|0x008:'48'
	// <0x7ac8dd>|0x014|0x00c:'49'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_fire_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7ac7f0>|0x000|0x000:'55'
	// <0x7ac80f>|0x01f|0x01f:'56'
	// <0x7ac839>|0x049|0x02a:'57'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7ac719>|0x000|0x000:'63'
	// <0x7ac720>|0x007|0x007:'64'
	// 1
	// <0x7ac727>|0x00e|0x007:'66'
	// <0x7ac737>|0x01e|0x010:'67'
	// <0x7ac769>|0x050|0x032:'68'
	// 1
	// 2
	// 3
	// <0x7ac775>|0x05c|0x00c:'72'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_fire_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// u32 							user_animation_index
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7ac5c1>|0x000|0x000:'77'
	// <0x7ac5c7>|0x006|0x006:'78'
	// 1
	// <0x7ac5d7>|0x016|0x010:'80'
	// <0x7ac5e3>|0x022|0x00c:'81'
	// <0x7ac603>|0x042|0x020:'82'
	// <0x7ac65c>|0x09b|0x059:'83'
	// 1
	// 2
	// <0x7ac66c>|0x0ab|0x010:'86'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// <0x7ac67a>|0x0b9|0x00e:'98'
	// <0x7ac6e8>|0x127|0x06e:'99'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_fire_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_fire_state* weapon_core_state_cook_template<double_barreled_weapon_core_fire_state>::new_object(
	vostok::mutable_buffer				buffer,
	weapon_state_creation_params const*	params,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
{
	return NULL;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x7ac549>|0x000|0x000:'111'
	// ******
}

} // namespace survarium
