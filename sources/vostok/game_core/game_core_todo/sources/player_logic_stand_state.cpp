////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_stand_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_stand_state::player_logic_stand_state(survarium::weapon_user_animations_selector&)
player_logic_stand_state::player_logic_stand_state( weapon_user_animations_selector& owner )
{
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::player_logic_stand_state::movement_lexeme(vostok::mutable_buffer&, const unsigned int, const vostok::animation::body_part_masks_enum, const bool, const bool, const bool) const
vostok::animation::mixing::animation_lexeme player_logic_stand_state::movement_lexeme(
	vostok::mutable_buffer&				buffer,
	u32									animation_index,
	vostok::animation::body_part_masks_enum	bones_mask,
	bool								is_aimed,
	bool								is_third_view,
	bool								is_firing) const
{
	// LOCALS
	// vostok::animation::linear_interpolator interpolator
	// u32 							main_animation_index
	// vostok::animation::mixing::animation_lexeme movement_lexeme
	// ******

	// FUNCTION BODY
	// <0x780971>|0x000|0x000:'44'
	// <0x78099c>|0x02b|0x02b:'45'
	// <0x7809ae>|0x03d|0x012:'46'
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
	// 14
	// <0x7809c1>|0x050|0x013:'61'
	// 1
	// <0x780b94>|0x223|0x1d3:'63'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::player_logic_stand_state::get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const
vostok::animation::mixing::expression player_logic_stand_state::get_recoil_animation_lexeme(
	animation_type_enum					animation_index,
	bool								aimed,
	float								coeff,
	vostok::animation::base_interpolator const&	interpolator,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	u32									additivity_priority,
	fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> const&	time_calculator) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme_parameters recoil_lexeme_parameters
	// vostok::animation::mixing::animation_lexeme lexeme
	// float 						start_animation_interval_time
	// pcstr 						additive_animation_id
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> additive_animation
	// ******

	// FUNCTION BODY
	// <0x780720>|0x000|0x000:'77'
	// <0x78076f>|0x04f|0x04f:'78'
	// <0x780825>|0x105|0x0b6:'79'
	// <0x780837>|0x117|0x012:'80'
	// <0x78084e>|0x12e|0x017:'81'
	// <0x780860>|0x140|0x012:'82'
	// <0x780872>|0x152|0x012:'83'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x7808a3>|0x183|0x031:'91'
	// <0x780918>|0x1f8|0x075:'92'
	// <0x780929>|0x209|0x011:'93'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::player_logic_stand_state::look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression player_logic_stand_state::look_expression(
	vostok::mutable_buffer&				buffer,
	u32									movement_animation_index,
	bool								is_aimed,
	bool								is_third_view,
	weapon_animation_parameters const&	weapon_parameters,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::expression result
	// pcstr 						look_animation_id
	// weapon_core& 				weapon
	// vostok::animation::instant_interpolator interpolator
	// vostok::animation::mixing::animation_lexeme_parameters look_lexeme_parameters
	// float 						start_animation_interval_time
	// vostok::animation::mixing::animation_lexeme look_lexeme
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> look_animation
	// vostok::animation::linear_interpolator l_interpolator
	// animation_type_enum 			animation_type
	// vostok::animation::mixing::expression expression<1>
	// vostok::animation::mixing::expression expression<1>
	// vostok::animation::mixing::expression expression<1>
	// ******

	// SKIPPED BLOCKS
	// <0x780e51><1>
	// <0x780ef1><1>
	// <0x780f91><1>
	// ******

	// FUNCTION BODY
	// <0x780bd1>|0x000|0x000:'105'
	// <0x780bdc>|0x00b|0x00b:'106'
	// <0x780bec>|0x01b|0x010:'107'
	// <0x780bf5>|0x024|0x009:'108'
	// <0x780c3a>|0x069|0x045:'109'
	// <0x780ce3>|0x112|0x0a9:'110'
	// 1
	// <0x780cf5>|0x124|0x012:'112'
	// <0x780d0e>|0x13d|0x019:'113'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x780d47>|0x176|0x039:'119'
	// 1
	// <0x780dd1>|0x200|0x08a:'121'
	// <0x780de2>|0x211|0x011:'122'
	// 1
	// <0x780df3>|0x222|0x011:'124'
	// 1
	// <0x780e3e>|0x26d|0x04b:'126'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x780e57>|0x286|0x019:'134'
	// <0x780e9f>|0x2ce|0x048:'135'
	// <0x780ed4>|0x303|0x035:'136'
	// 1
	// <0x780edf>|0x30e|0x00b:'138'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x780ef7>|0x326|0x018:'146'
	// <0x780f3e>|0x36d|0x047:'147'
	// <0x780f73>|0x3a2|0x035:'148'
	// 1
	// <0x780f7e>|0x3ad|0x00b:'150'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x780f97>|0x3c6|0x019:'158'
	// <0x780fdf>|0x40e|0x048:'159'
	// <0x781014>|0x443|0x035:'160'
	// 1
	// <0x78101f>|0x44e|0x00b:'162'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_stand_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> player_logic_stand_state::selected_animations( vostok::mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// LOCALS
	// u32 							movement_animation_index
	// vostok::animation::mixing::animation_lexeme main_lexeme
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x77009a }, type_index: TypeIndex(0x916f) })
	// ******

	// FUNCTION BODY
	// <0x781081>|0x000|0x000:'171'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x7810ab>|0x02a|0x02a:'177'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x7810e4>|0x063|0x039:'183'
	// ******
}

} // namespace survarium
