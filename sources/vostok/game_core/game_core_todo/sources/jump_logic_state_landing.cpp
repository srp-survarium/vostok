////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_landing.h"

namespace survarium {

// STATE[STUB]
// survarium::jump_logic_state_landing::jump_logic_state_landing(survarium::jump_logic&)
jump_logic_state_landing::jump_logic_state_landing( jump_logic& owner )
{
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::jump_logic_state_landing::selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> jump_logic_state_landing::selected_animations(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> const&	look_calculator,
	weapon_animation_parameters const&	weapon_parameters)
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme main_lexeme
	// vostok::animation::mixing::animation_lexeme look_lexeme
	// ******

	// FUNCTION BODY
	// <0x58d3f1>|0x000|0x000:'34'
	// <0x58d436>|0x045|0x045:'35'
	// <0x58d45c>|0x06b|0x026:'36'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_landing::get_main_lexeme(vostok::mutable_buffer&, const bool, const vostok::animation::body_part_masks_enum)
vostok::animation::mixing::animation_lexeme jump_logic_state_landing::get_main_lexeme( vostok::mutable_buffer& buffer, bool is_third_view, vostok::animation::body_part_masks_enum bones_mask )
{
	// LOCALS
	// pcstr 						caption
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> move_animation<1>
	// vostok::fixed_vector<vostok::animation::mixing::animation_interval,2> intervals<1>
	// bool 						landing_to_left_leg<1>
	// ******

	// SKIPPED BLOCKS
	// <0x58d17e><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::fixed_vector<vostok::animation::mixing::animation_interval,2>
	// 	two_anim_intervals_type;

	// ******

	// FUNCTION BODY
	// <0x58d121>|0x000|0x000:'43'
	// <0x58d159>|0x038|0x038:'44'
	// 1
	// <0x58d174>|0x053|0x01b:'46'
	// <0x58d184>|0x063|0x010:'47'
	// <0x58d19b>|0x07a|0x017:'48'
	// 1
	// <0x58d1a7>|0x086|0x00c:'50'
	// <0x58d1c3>|0x0a2|0x01c:'51'
	// <0x58d1d2>|0x0b1|0x00f:'52'
	// <0x58d1de>|0x0bd|0x00c:'53'
	// <0x58d22b>|0x10a|0x04d:'54'
	// <0x58d253>|0x132|0x028:'55'
	// <0x58d25d>|0x13c|0x00a:'56'
	// <0x58d25f>|0x13e|0x002:'57'
	// <0x58d287>|0x166|0x028:'58'
	// <0x58d2a9>|0x188|0x022:'59'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// <0x58d2b3>|0x192|0x00a:'69'
	// <0x58d370>|0x24f|0x0bd:'70'
	// <0x58d372>|0x251|0x002:'71'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x58d37c>|0x25b|0x00a:'78'
	// 1
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_landing::get_look_lexeme(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
vostok::animation::mixing::animation_lexeme jump_logic_state_landing::get_look_lexeme(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> const&	look_calculator,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation)
{
	// LOCALS
	// pcstr 						look_animation_id
	// vostok::animation::mixing::animation_lexeme_parameters parameters
	// float 						start_animation_interval_time
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> look_animation
	// ******

	// FUNCTION BODY
	// <0x58d030>|0x000|0x000:'87'
	// <0x58d049>|0x019|0x019:'88'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x58d05c>|0x02c|0x013:'95'
	// <0x58d072>|0x042|0x016:'96'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x58d099>|0x069|0x027:'104'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_landing::initialize()
void jump_logic_state_landing::initialize( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57bf4d }, type_index: TypeIndex(0x9160) })
	// ******

	// FUNCTION BODY
	// 1
	// <0x58cf3a>|0x000|0x000:'110'
	// <0x58cf4f>|0x015|0x015:'111'
	// <0x58cfef>|0x0b5|0x0a0:'112'
	// <0x58d00d>|0x0d3|0x01e:'113'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_landing::finalize()
void jump_logic_state_landing::finalize( )
{
	// FUNCTION BODY
	// 1
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::jump_logic_state_landing::on_interval_end(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum jump_logic_state_landing::on_interval_end( vostok::animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// <0x58ce59>|0x000|0x000:'126'
	// 1
	// 2
	// <0x58ceb3>|0x05a|0x05a:'129'
	// <0x58ceba>|0x061|0x007:'130'
	// <0x58ced9>|0x080|0x01f:'131'
	// 1
	// 2
	// 3
	// <0x58cee0>|0x087|0x007:'135'
	// ******
}

} // namespace survarium
