////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "double_barreled_weapon_core_hide_state.h"

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_hide_state::double_barreled_weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
double_barreled_weapon_core_hide_state::double_barreled_weapon_core_hide_state(
	weapon_core&						weapon,
	float								animation_timescale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	bool&								is_shown)
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
	// <0x7ad567><2>
	// <0x7ad57f><3>
	// <0x7ad5f4><3>
	// ******

	// FUNCTION BODY
	// <0x7ad4fc>|0x000|0x000:'27'
	// <0x7ad527>|0x02b|0x02b:'28'
	// <0x7ad54a>|0x04e|0x023:'29'
	// <0x7ad551>|0x055|0x007|[1]:'30'
	// <0x7ad569>|0x06d|0x018:'31'
	// <0x7ad581>|0x085|0x018:'32'
	// <0x7ad599>|0x09d|0x018:'33'
	// <0x7ad5d5>|0x0d9|0x03c:'34'
	// <0x7ad5d7>|0x0db|0x002:'35'
	// <0x7ad5d9>|0x0dd|0x002:'36'
	// <0x7ad5de>|0x0e2|0x005|[2]:'37'
	// <0x7ad5f6>|0x0fa|0x018:'38'
	// <0x7ad60e>|0x112|0x018:'39'
	// <0x7ad63f>|0x143|0x031:'40'
	// <0x7ad641>|0x145|0x002:'41'
	// <0x7ad643>|0x147|0x002:'42'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_hide_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_hide_state::weapon_and_hands_expression(
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
	// <0x7ad900>|0x000|0x000:'47'
	// <0x7ad91f>|0x01f|0x01f:'48'
	// <0x7ad949>|0x049|0x02a:'49'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_hide_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_hide_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							weapon_animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7ad819>|0x000|0x000:'54'
	// 1
	// <0x7ad825>|0x00c|0x00c:'56'
	// <0x7ad82c>|0x013|0x007:'57'
	// <0x7ad833>|0x01a|0x007:'58'
	// 1
	// 2
	// <0x7ad83a>|0x021|0x007:'61'
	// <0x7ad84e>|0x035|0x014:'62'
	// <0x7ad858>|0x03f|0x00a:'63'
	// <0x7ad886>|0x06d|0x02e:'64'
	// 1
	// 2
	// 3
	// <0x7ad892>|0x079|0x00c:'68'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_hide_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_hide_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// pcstr[2] 					animation_captions
	// u32 							user_state_index
	// ******

	// FUNCTION BODY
	// <0x7ad741>|0x000|0x000:'73'
	// <0x7ad747>|0x006|0x006:'74'
	// 1
	// 2
	// <0x7ad757>|0x016|0x010:'77'
	// 1
	// 2
	// <0x7ad763>|0x022|0x00c:'80'
	// <0x7ad76a>|0x029|0x007:'81'
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
	// 12
	// 13
	// <0x7ad771>|0x030|0x007:'95'
	// <0x7ad7ea>|0x0a9|0x079:'96'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_hide_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_hide_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_hide_state* weapon_core_state_cook_template<double_barreled_weapon_core_hide_state>::new_object(
	vostok::mutable_buffer				buffer,
	weapon_state_creation_params const*	params,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
{
	// LOCALS
	// float 						time_scale
	// float 						weapon_anim_length
	// float 						user_anim_length
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x7ad669>|0x000|0x000:'103'
	// <0x7ad694>|0x02b|0x02b:'104'
	// <0x7ad6c4>|0x05b|0x030:'105'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x7ad6d3>|0x06a|0x00f:'112'
	// ******
}

} // namespace survarium
