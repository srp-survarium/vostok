////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "double_barreled_weapon_core_show_state.h"

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_show_state::double_barreled_weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
double_barreled_weapon_core_show_state::double_barreled_weapon_core_show_state(
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
	// <0x7adaa7><2>
	// <0x7adabf><3>
	// <0x7adb34><3>
	// ******

	// FUNCTION BODY
	// <0x7ada3c>|0x000|0x000:'27'
	// <0x7ada67>|0x02b|0x02b:'28'
	// <0x7ada8a>|0x04e|0x023:'29'
	// <0x7ada91>|0x055|0x007|[1]:'30'
	// <0x7adaa9>|0x06d|0x018:'31'
	// <0x7adac1>|0x085|0x018:'32'
	// <0x7adad9>|0x09d|0x018:'33'
	// <0x7adb15>|0x0d9|0x03c:'34'
	// <0x7adb17>|0x0db|0x002:'35'
	// <0x7adb19>|0x0dd|0x002:'36'
	// <0x7adb1e>|0x0e2|0x005|[2]:'37'
	// <0x7adb36>|0x0fa|0x018:'38'
	// <0x7adb4e>|0x112|0x018:'39'
	// <0x7adb7f>|0x143|0x031:'40'
	// <0x7adb81>|0x145|0x002:'41'
	// <0x7adb83>|0x147|0x002:'42'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_show_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7ade40>|0x000|0x000:'47'
	// <0x7ade5f>|0x01f|0x01f:'48'
	// <0x7ade89>|0x049|0x02a:'49'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_show_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_show_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							weapon_animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7add59>|0x000|0x000:'54'
	// 1
	// <0x7add65>|0x00c|0x00c:'56'
	// <0x7add6c>|0x013|0x007:'57'
	// <0x7add73>|0x01a|0x007:'58'
	// 1
	// 2
	// <0x7add7a>|0x021|0x007:'61'
	// <0x7add8e>|0x035|0x014:'62'
	// <0x7add98>|0x03f|0x00a:'63'
	// <0x7addc6>|0x06d|0x02e:'64'
	// 1
	// 2
	// 3
	// <0x7addd2>|0x079|0x00c:'68'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_show_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_show_state::get_user_hands_expression(
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
	// <0x7adc81>|0x000|0x000:'73'
	// <0x7adc87>|0x006|0x006:'74'
	// 1
	// 2
	// <0x7adc97>|0x016|0x010:'77'
	// <0x7adca5>|0x024|0x00e:'78'
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
	// <0x7adcb1>|0x030|0x00c:'90'
	// <0x7add2a>|0x0a9|0x079:'91'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_show_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_show_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_show_state* weapon_core_state_cook_template<double_barreled_weapon_core_show_state>::new_object(
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
	// <0x7adba9>|0x000|0x000:'98'
	// <0x7adbd4>|0x02b|0x02b:'99'
	// <0x7adc04>|0x05b|0x030:'100'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x7adc13>|0x06a|0x00f:'107'
	// ******
}

} // namespace survarium
