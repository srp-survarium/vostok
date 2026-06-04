////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_crouch_state.h"
#include <vostok/game_core/base_player.h>

namespace survarium {

// STATE[100%|DONE]
player_logic_crouch_state::player_logic_crouch_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_crouch )
{
}

// STATE[100%|DONE]
void player_logic_crouch_state::initialize( )
{
	m_user->crouch( );
}

// STATE[100%|DONE]
void player_logic_crouch_state::finalize( )
{
	m_user->stand_up( );
}

// STATE[0%|PARTIAL]: UNREACHABLE stub. Real body (target @0x57efb0) is the lexeme
// machinery: builds an animation_lexeme via weapon_user_animations_container::
// get_animation_impl<27,6>, crouch_animations_captions static, linear_interpolator,
// and the animation_lexeme_parameters builder. Same machinery wall as
// jump_logic_state_start::get_main_lexeme (needs get_animation_impl bodies +
// animation_lexeme_parameters chain), out of this unit. Decoded asm in the .md.
// vostok::animation::mixing::animation_lexeme survarium::player_logic_crouch_state::movement_lexeme(vostok::mutable_buffer&, const unsigned int, const vostok::animation::body_part_masks_enum, const bool, const bool, const bool) const
animation::mixing::animation_lexeme player_logic_crouch_state::movement_lexeme(
	mutable_buffer&						buffer,
	u32									animation_index,
	animation::body_part_masks_enum		bones_mask,
	bool								is_aimed,
	bool								is_third_view,
	bool								is_firing
) const
{
	// LOCALS
	// u32 							move_animation_index
	// animation::linear_interpolator interpolator
	// animation::mixing::animation_lexeme movement_lexeme
	// ******

	// FUNCTION BODY
	// <0x58efc1>|0x011|+0x02b:'54'
	// <0x58efec>|0x03c|+0x013:'55'
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
	// <12>
	// <13>
	// <0x58efff>|0x04f|+0x1b8:'70'
	// <0>
	// <0x58f1b7>|0x207|+0x021:'72'
	// ******
	UNREACHABLE_CODE( );
}

// STATE[0%|PARTIAL]: UNREACHABLE stub. Real body (target @0x57ed50) is recoil lexeme
// machinery (animation_lexeme_parameters builder, managed_resource_ptr additive
// animation, interpolators). Same lexeme/operator machinery wall, out of this unit.
// Not anchored (only selected_animations would reach it, itself a blocked stub). Decoded
// asm in the .md.
// vostok::animation::mixing::expression survarium::player_logic_crouch_state::get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const
animation::mixing::expression player_logic_crouch_state::get_recoil_animation_lexeme(
	animation_type_enum					animation_index,
	bool								aimed,
	float								coeff,
	animation::base_interpolator const&	interpolator,
	mutable_buffer&						buffer,
	bool								is_third_view,
	u32									additivity_priority,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	time_calculator
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme_parameters recoil_lexeme_parameters
	// animation::mixing::animation_lexeme lexeme
	// float 						start_animation_interval_time
	// pcstr 						additive_animation_id
	// resources::managed_resource_ptr additive_animation
	// ******

	// FUNCTION BODY
	// <0x58ed60>|0x010|+0x042:'86'
	// <0x58eda2>|0x052|+0x0b6:'87'
	// <0x58ee58>|0x108|+0x012:'88'
	// <0x58ee6a>|0x11a|+0x017:'89'
	// <0x58ee81>|0x131|+0x04c:'90'
	// <0x58eecd>|0x17d|+0x012:'91'
	// <0x58eedf>|0x18f|+0x012:'92'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x58eef1>|0x1a1|+0x075:'100'
	// <0x58ef66>|0x216|+0x011:'101'
	// <0x58ef77>|0x227|+0x02f:'102'
	// ******
	UNREACHABLE_CODE( );
}

// STATE[0%|PARTIAL]: UNREACHABLE stub. Real body (target @0x57f1e0) is the look-lexeme
// machinery: get_animation_impl<27,6>, crouch_animations_captions, instant/linear
// interpolators, weapon_user_animations_selector::look_time_factor/look_time_calculator,
// animation_lexeme_parameters builder, three near-identical lexeme-build blocks summed via
// the expression operator+ overload (the same operator+(expression&,expression const&) that
// blocks selected_animations). Lexeme/operator machinery wall, out of this unit. Decoded asm
// in the .md.
// vostok::animation::mixing::expression survarium::player_logic_crouch_state::look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression player_logic_crouch_state::look_expression(
	mutable_buffer&						buffer,
	u32									movement_animation_index,
	bool								is_aimed,
	bool								is_third_view,
	weapon_animation_parameters const&	weapon_parameters,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::expression result
	// pcstr 						look_animation_id
	// weapon_core& 				weapon
	// animation::instant_interpolator interpolator
	// animation::mixing::animation_lexeme_parameters look_lexeme_parameters
	// float 						start_animation_interval_time
	// animation::mixing::animation_lexeme look_lexeme
	// resources::managed_resource_ptr look_animation
	// animation::linear_interpolator l_interpolator
	// animation_type_enum 			animation_type
	// animation::mixing::expression expression<1>
	// animation::mixing::expression expression<1>
	// animation::mixing::expression expression<1>
	// ******

	// SKIPPED BLOCKS
	// <0x58f45f><1>
	// <0x58f4ff><1>
	// <0x58f59f><1>
	// ******

	// FUNCTION BODY
	// <0x58f1f1>|0x011|+0x00b:'114'
	// <0x58f1fc>|0x01c|+0x010:'115'
	// <0x58f20c>|0x02c|+0x009:'116'
	// <0x58f215>|0x035|+0x045:'117'
	// <0x58f25a>|0x07a|+0x0a9:'118'
	// <0>
	// <0x58f303>|0x123|+0x019:'120'
	// <0x58f31c>|0x13c|+0x039:'121'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x58f355>|0x175|+0x08a:'127'
	// <0>
	// <0x58f3df>|0x1ff|+0x011:'129'
	// <0x58f3f0>|0x210|+0x011:'130'
	// <0>
	// <0x58f401>|0x221|+0x04b:'132'
	// <0>
	// <0x58f44c>|0x26c|+0x019:'134'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x58f465>|0x285|+0x048:'142'
	// <0x58f4ad>|0x2cd|+0x035:'143'
	// <0x58f4e2>|0x302|+0x00b:'144'
	// <0>
	// <0x58f4ed>|0x30d|+0x018:'146'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x58f505>|0x325|+0x047:'154'
	// <0x58f54c>|0x36c|+0x035:'155'
	// <0x58f581>|0x3a1|+0x00b:'156'
	// <0>
	// <0x58f58c>|0x3ac|+0x019:'158'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x58f5a5>|0x3c5|+0x048:'166'
	// <0x58f5ed>|0x40d|+0x035:'167'
	// <0x58f622>|0x442|+0x00b:'168'
	// <0>
	// <0x58f62d>|0x44d|+0x044:'170'
	// ******
	UNREACHABLE_CODE( );
}

// STATE[6.47%|PARTIAL]: UNREACHABLE stub (emitted, paired). Real body is verified from
// the target @0x57f680 (decoded in the .md):
//   u32 movement_animation_index;
//   if ( (*m_user->damage_model()).broken_legs_count() > 1 ) movement_animation_index = 0;
//   else movement_animation_index = player_logic_base_state::movement_animation_index( m_user->input() );
//   animation::mixing::animation_lexeme main_lexeme = movement_lexeme( buffer,
//       movement_animation_index, weapon_parameters.body_part_mask, weapon_parameters.is_aimed,
//       is_third_view, weapon_parameters.is_firing );
//   return std::make_pair( look_expression( buffer, movement_animation_index,
//       weapon_parameters.is_aimed, is_third_view, weapon_parameters, main_lexeme )
//       + animation::mixing::expression( main_lexeme ), main_lexeme );
// BLOCKED: target calls free `mixing::operator+(expression&, expression const&) -> expression`,
// but only the template `operator+(T1&,T2&) -> addition_lexeme&` is declared
// (mixing_addition_lexeme.h) -> C2678. Adding the expression-returning overload is the
// shared lexeme/operator+ machinery wall (same as jump_logic_state_start), out of this unit.
// Also depends on movement_lexeme/look_expression, themselves on the same machinery wall.
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_crouch_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// LOCALS
	// u32 							movement_animation_index
	// animation::mixing::animation_lexeme main_lexeme
	// ******

	// CALL SITE INFO
	// <0x58f6a8> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// <0x58f6fe> -> player_input const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x58f691>|0x011|+0x08a:'179'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x58f71b>|0x09b|+0x039:'185'
	// <0>
	// <0x58f754>|0x0d4|+0x0a1:'187'
	// ******
	UNREACHABLE_CODE( );
}

} // namespace survarium
