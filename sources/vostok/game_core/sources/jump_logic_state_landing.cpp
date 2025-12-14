////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_landing.h"

namespace survarium {

// STATE[STUB]
// survarium::jump_logic_state_landing::jump_logic_state_landing(survarium::jump_logic&)
jump_logic_state_landing::jump_logic_state_landing( jump_logic& owner ) : jump_logic_base_state( owner )
{
	// FUNCTION BODY
	// <0x58cef0>|0x000|+0x028:'26'	{
	// <0x58cf18>|0x028|      :'27'	}
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::jump_logic_state_landing::selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_landing::selected_animations(
	mutable_buffer&						buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	weapon_animation_parameters const&	weapon_parameters
)
{
	// LOCALS
	// animation::mixing::animation_lexeme main_lexeme
	// animation::mixing::animation_lexeme look_lexeme
	// ******

	// FUNCTION BODY
	// <0x58d3f1>|0x011|+0x045:'34'
	// <0x58d436>|0x056|+0x026:'35'
	// <0x58d45c>|0x07c|+0x088:'36'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_landing::get_main_lexeme(vostok::mutable_buffer&, const bool, const vostok::animation::body_part_masks_enum)
animation::mixing::animation_lexeme jump_logic_state_landing::get_main_lexeme( mutable_buffer& buffer, bool is_third_view, animation::body_part_masks_enum bones_mask )
{
	// LOCALS
	// pcstr 						caption
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> move_animation<1>
	// fixed_vector<animation::mixing::animation_interval,2> intervals<1>
	// bool 						landing_to_left_leg<1>
	// ******

	// SKIPPED BLOCKS
	// <0x58d17e><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector<animation::mixing::animation_interval,2>
	// 	two_anim_intervals_type;

	// ******

	// FUNCTION BODY
	// <0x58d121>|0x011|+0x038:'43'
	// <0x58d159>|0x049|+0x01b:'44'
	// <0>
	// <0x58d174>|0x064|+0x010:'46'
	// <0x58d184>|0x074|+0x017:'47'
	// <0x58d19b>|0x08b|+0x00c:'48'
	// <0>
	// <0x58d1a7>|0x097|+0x01c:'50'
	// <0x58d1c3>|0x0b3|+0x00f:'51'
	// <0x58d1d2>|0x0c2|+0x00c:'52'
	// <0x58d1de>|0x0ce|+0x04d:'53'
	// <0x58d22b>|0x11b|+0x028:'54'
	// <0x58d253>|0x143|+0x00a:'55'
	// <0x58d25d>|0x14d|+0x002:'56'
	// <0x58d25f>|0x14f|+0x028:'57'
	// <0x58d287>|0x177|+0x022:'58'
	// <0x58d2a9>|0x199|+0x00a:'59'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x58d2b3>|0x1a3|+0x0bd:'69'
	// <0x58d370>|0x260|+0x002:'70'
	// <0x58d372>|0x262|+0x00a:'71'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58d37c>|0x26c|+0x05a:'78'
	// <0>
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_landing::get_look_lexeme(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
animation::mixing::animation_lexeme jump_logic_state_landing::get_look_lexeme(
	mutable_buffer&						buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	animation::mixing::animation_lexeme&	weight_driving_animation
)
{
	// LOCALS
	// pcstr 						look_animation_id
	// animation::mixing::animation_lexeme_parameters parameters
	// float 						start_animation_interval_time
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> look_animation
	// ******

	// FUNCTION BODY
	// <0x58d030>|0x010|+0x019:'87'
	// <0x58d049>|0x029|+0x013:'88'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58d05c>|0x03c|+0x016:'95'
	// <0x58d072>|0x052|+0x027:'96'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x58d099>|0x079|+0x06d:'104'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_landing::initialize()
void jump_logic_state_landing::initialize( )
{
	// CALL SITE INFO
	// <0x58cf4d> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0>
	// <0x58cf3a>|0x00a|+0x015:'110'
	// <0x58cf4f>|0x01f|+0x0a0:'111'
	// <0x58cfef>|0x0bf|+0x01e:'112'
	// <0x58d00d>|0x0dd|+0x007:'113'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_landing::finalize()
void jump_logic_state_landing::finalize( )
{
	// FUNCTION BODY
	// <0x58ce40>|0x000|+0x007:'117'	{
	// <0>
	// <0x58ce47>|0x007|      :'119'	}
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::jump_logic_state_landing::on_interval_end(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum jump_logic_state_landing::on_interval_end( animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x58ce59>|0x009|+0x05a:'126'
	// <0>
	// <1>
	// <0x58ceb3>|0x063|+0x007:'129'
	// <0x58ceba>|0x06a|+0x01f:'130'
	// <0x58ced9>|0x089|+0x007:'131'
	// <0>
	// <1>
	// <2>
	// <0x58cee0>|0x090|+0x002:'135'
	// ******
}

} // namespace survarium
