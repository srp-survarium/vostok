////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_stand_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_stand_state::player_logic_stand_state(survarium::weapon_user_animations_selector&)
player_logic_stand_state::player_logic_stand_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_stand )
{
	// FUNCTION BODY
	// <0x7806e0>|0x000|+0x01e:'32'	{
	// <0x7806fe>|0x01e|      :'33'	}
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::player_logic_stand_state::movement_lexeme(vostok::mutable_buffer&, const unsigned int, const vostok::animation::body_part_masks_enum, const bool, const bool, const bool) const
animation::mixing::animation_lexeme player_logic_stand_state::movement_lexeme(
	mutable_buffer&						buffer,
	u32									animation_index,
	animation::body_part_masks_enum		bones_mask,
	bool								is_aimed,
	bool								is_third_view,
	bool								is_firing
) const
{
	// LOCALS
	// animation::linear_interpolator interpolator
	// u32 							main_animation_index
	// animation::mixing::animation_lexeme movement_lexeme
	// ******

	// FUNCTION BODY
	// <0x780971>|0x011|+0x02b:'44'
	// <0x78099c>|0x03c|+0x012:'45'
	// <0x7809ae>|0x04e|+0x013:'46'
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
	// <0x7809c1>|0x061|+0x1d3:'61'
	// <0>
	// <0x780b94>|0x234|+0x021:'63'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::player_logic_stand_state::get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const
animation::mixing::expression player_logic_stand_state::get_recoil_animation_lexeme(
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
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> additive_animation
	// ******

	// FUNCTION BODY
	// <0x780720>|0x010|+0x04f:'77'
	// <0x78076f>|0x05f|+0x0b6:'78'
	// <0x780825>|0x115|+0x012:'79'
	// <0x780837>|0x127|+0x017:'80'
	// <0x78084e>|0x13e|+0x012:'81'
	// <0x780860>|0x150|+0x012:'82'
	// <0x780872>|0x162|+0x031:'83'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x7808a3>|0x193|+0x075:'91'
	// <0x780918>|0x208|+0x011:'92'
	// <0x780929>|0x219|+0x02f:'93'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::player_logic_stand_state::look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression player_logic_stand_state::look_expression(
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
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> look_animation
	// animation::linear_interpolator l_interpolator
	// animation_type_enum 			animation_type
	// animation::mixing::expression expression<1>
	// animation::mixing::expression expression<1>
	// animation::mixing::expression expression<1>
	// ******

	// SKIPPED BLOCKS
	// <0x780e51><1>
	// <0x780ef1><1>
	// <0x780f91><1>
	// ******

	// FUNCTION BODY
	// <0x780bd1>|0x011|+0x00b:'105'
	// <0x780bdc>|0x01c|+0x010:'106'
	// <0x780bec>|0x02c|+0x009:'107'
	// <0x780bf5>|0x035|+0x045:'108'
	// <0x780c3a>|0x07a|+0x0a9:'109'
	// <0x780ce3>|0x123|+0x012:'110'
	// <0>
	// <0x780cf5>|0x135|+0x019:'112'
	// <0x780d0e>|0x14e|+0x039:'113'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x780d47>|0x187|+0x08a:'119'
	// <0>
	// <0x780dd1>|0x211|+0x011:'121'
	// <0x780de2>|0x222|+0x011:'122'
	// <0>
	// <0x780df3>|0x233|+0x04b:'124'
	// <0>
	// <0x780e3e>|0x27e|+0x019:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x780e57>|0x297|+0x048:'134'
	// <0x780e9f>|0x2df|+0x035:'135'
	// <0x780ed4>|0x314|+0x00b:'136'
	// <0>
	// <0x780edf>|0x31f|+0x018:'138'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x780ef7>|0x337|+0x047:'146'
	// <0x780f3e>|0x37e|+0x035:'147'
	// <0x780f73>|0x3b3|+0x00b:'148'
	// <0>
	// <0x780f7e>|0x3be|+0x019:'150'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x780f97>|0x3d7|+0x048:'158'
	// <0x780fdf>|0x41f|+0x035:'159'
	// <0x781014>|0x454|+0x00b:'160'
	// <0>
	// <0x78101f>|0x45f|+0x044:'162'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_stand_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_stand_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// LOCALS
	// u32 							movement_animation_index
	// animation::mixing::animation_lexeme main_lexeme
	// ******

	// CALL SITE INFO
	// <0x78109a> -> player_input const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x781081>|0x011|+0x02a:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7810ab>|0x03b|+0x039:'177'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7810e4>|0x074|+0x0a1:'183'
	// ******
}

} // namespace survarium
