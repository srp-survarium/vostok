////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_show_state.h>

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_show_state::double_barreled_weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
double_barreled_weapon_core_show_state::double_barreled_weapon_core_show_state(
	weapon_core&						weapon,
	float								animation_timescale,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	bool&								is_shown
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
	// <0x7adaa7><2>
	// <0x7adabf><3>
	// <0x7adb34><3>
	// ******

	// FUNCTION BODY
	// <0x7ada3c>|0x06c|+0x02b:'27'
	// <0x7ada67>|0x097|+0x023:'28'
	// <0x7ada8a>|0x0ba|+0x007:'29'
	// <0x7ada91>|0x0c1|+0x018|[1]:'30'
	// <0x7adaa9>|0x0d9|+0x018:'31'
	// <0x7adac1>|0x0f1|+0x018:'32'
	// <0x7adad9>|0x109|+0x03c:'33'
	// <0x7adb15>|0x145|+0x002:'34'
	// <0x7adb17>|0x147|+0x002:'35'
	// <0x7adb19>|0x149|+0x005:'36'
	// <0x7adb1e>|0x14e|+0x018|[2]:'37'
	// <0x7adb36>|0x166|+0x018:'38'
	// <0x7adb4e>|0x17e|+0x031:'39'
	// <0x7adb7f>|0x1af|+0x002:'40'
	// <0x7adb81>|0x1b1|+0x002:'41'
	// <0x7adb83>|0x1b3|+0x00c:'42'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_show_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7ade40>|0x010|+0x01f:'47'
	// <0x7ade5f>|0x02f|+0x02a:'48'
	// <0x7ade89>|0x059|+0x07a:'49'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_show_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// u32 							weapon_animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7add59>|0x009|+0x00c:'54'
	// <0>
	// <0x7add65>|0x015|+0x007:'56'
	// <0x7add6c>|0x01c|+0x007:'57'
	// <0x7add73>|0x023|+0x007:'58'
	// <0>
	// <1>
	// <0x7add7a>|0x02a|+0x014:'61'
	// <0x7add8e>|0x03e|+0x00a:'62'
	// <0x7add98>|0x048|+0x02e:'63'
	// <0x7addc6>|0x076|+0x00c:'64'
	// <0>
	// <1>
	// <2>
	// <0x7addd2>|0x082|+0x04e:'68'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_show_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_show_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// pcstr[2] 					animation_captions
	// u32 							user_state_index
	// ******

	// FUNCTION BODY
	// <0x7adc81>|0x011|+0x006:'73'
	// <0x7adc87>|0x017|+0x010:'74'
	// <0>
	// <1>
	// <0x7adc97>|0x027|+0x00e:'77'
	// <0x7adca5>|0x035|+0x00c:'78'
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
	// <0x7adcb1>|0x041|+0x079:'90'
	// <0x7add2a>|0x0ba|+0x01c:'91'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_show_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_show_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_show_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_show_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count
)
{
	// LOCALS
	// float 						time_scale
	// float 						weapon_anim_length
	// float 						user_anim_length
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x7adba9>|0x009|+0x02b:'98'
	// <0x7adbd4>|0x034|+0x030:'99'
	// <0x7adc04>|0x064|+0x00f:'100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7adc13>|0x073|+0x053:'107'
	// ******
}

} // namespace survarium
