////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_crouch_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_crouch_state::player_logic_crouch_state(survarium::weapon_user_animations_selector&)
player_logic_crouch_state::player_logic_crouch_state( weapon_user_animations_selector& owner )
{
}

// STATE[STUB]
// void survarium::player_logic_crouch_state::initialize()
void player_logic_crouch_state::initialize( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57dd1a }, type_index: TypeIndex(0x9160) })
	// ******

	// FUNCTION BODY
	// <0x58ed07>|0x000|0x000:'37'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::player_logic_crouch_state::finalize()
void player_logic_crouch_state::finalize( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57dcfa }, type_index: TypeIndex(0x9160) })
	// ******

	// FUNCTION BODY
	// <0x58ece7>|0x000|0x000:'42'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::player_logic_crouch_state::movement_lexeme(vostok::mutable_buffer&, const unsigned int, const vostok::animation::body_part_masks_enum, const bool, const bool, const bool) const
vostok::animation::mixing::animation_lexeme player_logic_crouch_state::movement_lexeme(
	vostok::mutable_buffer&				buffer,
	u32									animation_index,
	vostok::animation::body_part_masks_enum	bones_mask,
	bool								is_aimed,
	bool								is_third_view,
	bool								is_firing) const
{
	// LOCALS
	// u32 							move_animation_index
	// vostok::animation::linear_interpolator interpolator
	// vostok::animation::mixing::animation_lexeme movement_lexeme
	// ******

	// FUNCTION BODY
	// <0x58efc1>|0x000|0x000:'54'
	// <0x58efec>|0x02b|0x02b:'55'
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
	// <0x58efff>|0x03e|0x013:'70'
	// 1
	// <0x58f1b7>|0x1f6|0x1b8:'72'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::player_logic_crouch_state::get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const
vostok::animation::mixing::expression player_logic_crouch_state::get_recoil_animation_lexeme(
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
	// <0x58ed60>|0x000|0x000:'86'
	// <0x58eda2>|0x042|0x042:'87'
	// <0x58ee58>|0x0f8|0x0b6:'88'
	// <0x58ee6a>|0x10a|0x012:'89'
	// <0x58ee81>|0x121|0x017:'90'
	// <0x58eecd>|0x16d|0x04c:'91'
	// <0x58eedf>|0x17f|0x012:'92'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x58eef1>|0x191|0x012:'100'
	// <0x58ef66>|0x206|0x075:'101'
	// <0x58ef77>|0x217|0x011:'102'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::player_logic_crouch_state::look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression player_logic_crouch_state::look_expression(
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
	// <0x58f45f><1>
	// <0x58f4ff><1>
	// <0x58f59f><1>
	// ******

	// FUNCTION BODY
	// <0x58f1f1>|0x000|0x000:'114'
	// <0x58f1fc>|0x00b|0x00b:'115'
	// <0x58f20c>|0x01b|0x010:'116'
	// <0x58f215>|0x024|0x009:'117'
	// <0x58f25a>|0x069|0x045:'118'
	// 1
	// <0x58f303>|0x112|0x0a9:'120'
	// <0x58f31c>|0x12b|0x019:'121'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x58f355>|0x164|0x039:'127'
	// 1
	// <0x58f3df>|0x1ee|0x08a:'129'
	// <0x58f3f0>|0x1ff|0x011:'130'
	// 1
	// <0x58f401>|0x210|0x011:'132'
	// 1
	// <0x58f44c>|0x25b|0x04b:'134'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x58f465>|0x274|0x019:'142'
	// <0x58f4ad>|0x2bc|0x048:'143'
	// <0x58f4e2>|0x2f1|0x035:'144'
	// 1
	// <0x58f4ed>|0x2fc|0x00b:'146'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x58f505>|0x314|0x018:'154'
	// <0x58f54c>|0x35b|0x047:'155'
	// <0x58f581>|0x390|0x035:'156'
	// 1
	// <0x58f58c>|0x39b|0x00b:'158'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x58f5a5>|0x3b4|0x019:'166'
	// <0x58f5ed>|0x3fc|0x048:'167'
	// <0x58f622>|0x431|0x035:'168'
	// 1
	// <0x58f62d>|0x43c|0x00b:'170'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_crouch_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> player_logic_crouch_state::selected_animations( vostok::mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// LOCALS
	// u32 							movement_animation_index
	// vostok::animation::mixing::animation_lexeme main_lexeme
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57e6a8 }, type_index: TypeIndex(0x916d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57e6fe }, type_index: TypeIndex(0x916f) })
	// ******

	// FUNCTION BODY
	// <0x58f691>|0x000|0x000:'179'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x58f71b>|0x08a|0x08a:'185'
	// 1
	// <0x58f754>|0x0c3|0x039:'187'
	// ******
}

} // namespace survarium
