////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_chamber_a_round_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_chamber_a_round_state::weapon_core_chamber_a_round_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_chamber_a_round_state::weapon_core_chamber_a_round_state(
	weapon_core&						weapon,
	float								animation_time_scale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
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

	// FUNCTION BODY
	// <0x7aa37e>|0x000|0x000:'28'
	// <0x7aa3a1>|0x023|0x023:'29'
	// <0x7aa3a8>|0x02a|0x007|[1]:'30'
	// <0x7aa3c0>|0x042|0x018:'31'
	// <0x7aa3d8>|0x05a|0x018:'32'
	// <0x7aa409>|0x08b|0x031:'33'
	// <0x7aa40b>|0x08d|0x002:'34'
	// <0x7aa40d>|0x08f|0x002|[2]:'35'
	// <0x7aa425>|0x0a7|0x018:'36'
	// <0x7aa43d>|0x0bf|0x018:'37'
	// <0x7aa46e>|0x0f0|0x031:'38'
	// <0x7aa470>|0x0f2|0x002:'39'
	// <0x7aa472>|0x0f4|0x002:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_chamber_a_round_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_chamber_a_round_state::weapon_and_hands_expression(
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
	// <0x7aa6a0>|0x000|0x000:'45'
	// <0x7aa6bf>|0x01f|0x01f:'46'
	// <0x7aa6e9>|0x049|0x02a:'47'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_chamber_a_round_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_chamber_a_round_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aa5f9>|0x000|0x000:'52'
	// <0x7aa600>|0x007|0x007:'53'
	// <0x7aa623>|0x02a|0x023:'54'
	// 1
	// 2
	// 3
	// <0x7aa62f>|0x036|0x00c:'58'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_chamber_a_round_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_chamber_a_round_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// pcstr[3] 					animation_captions
	// u32 							user_animation_index
	// ******

	// FUNCTION BODY
	// <0x7aa501>|0x000|0x000:'63'
	// <0x7aa507>|0x006|0x006:'64'
	// 1
	// <0x7aa517>|0x016|0x010:'66'
	// 1
	// <0x7aa529>|0x028|0x012:'68'
	// 1
	// <0x7aa535>|0x034|0x00c:'70'
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
	// <0x7aa54a>|0x049|0x015:'83'
	// <0x7aa5c3>|0x0c2|0x079:'84'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_chamber_a_round_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_chamber_a_round_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_chamber_a_round_state* weapon_core_state_cook_template<weapon_core_chamber_a_round_state>::new_object(
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
	// 6
	// <0x7aa499>|0x000|0x000:'97'
	// ******
}

} // namespace survarium
