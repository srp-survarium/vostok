////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_start.h"

namespace survarium {

// STATE[STUB]
// survarium::jump_logic_state_start::jump_logic_state_start(survarium::jump_logic&)
jump_logic_state_start::jump_logic_state_start( jump_logic& owner ) : jump_logic_base_state( owner )
{
	// FUNCTION BODY
	// <0x6f8bd0>|0x000|+0x037:'29'	{
	// <0x6f8c07>|0x037|      :'30'	}
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::jump_logic_state_start::selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_start::selected_animations(
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
	// <0x6f9221>|0x011|+0x045:'37'
	// <0x6f9266>|0x056|+0x026:'38'
	// <0x6f928c>|0x07c|+0x088:'39'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_start::get_main_lexeme(vostok::mutable_buffer&, const bool, const vostok::animation::body_part_masks_enum)
animation::mixing::animation_lexeme jump_logic_state_start::get_main_lexeme( mutable_buffer& buffer, bool is_third_view, animation::body_part_masks_enum bones_mask )
{
	// LOCALS
	// pcstr 						caption
	// fixed_vector<animation::mixing::animation_interval,2> intervals<1>
	// animation::linear_interpolator interpolator<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6f8f6d><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector<animation::mixing::animation_interval,2>
	// 	two_anim_intervals_type;

	// ******

	// FUNCTION BODY
	// <0x6f8f11>|0x011|+0x030:'46'
	// <0x6f8f41>|0x041|+0x013:'47'
	// <0>
	// <0x6f8f54>|0x054|+0x01f:'49'
	// <0x6f8f73>|0x073|+0x02e:'50'
	// <0x6f8fa1>|0x0a1|+0x00c:'51'
	// <0>
	// <0x6f8fad>|0x0ad|+0x01c:'53'
	// <0x6f8fc9>|0x0c9|+0x021:'54'
	// <0x6f8fea>|0x0ea|+0x053:'55'
	// <0x6f903d>|0x13d|+0x028:'56'
	// <0x6f9065>|0x165|+0x00a:'57'
	// <0x6f906f>|0x16f|+0x002:'58'
	// <0x6f9071>|0x171|+0x028:'59'
	// <0x6f9099>|0x199|+0x028:'60'
	// <0x6f90c1>|0x1c1|+0x00a:'61'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6f90cb>|0x1cb|+0x0bd:'71'
	// <0x6f9188>|0x288|+0x002:'72'
	// <0x6f918a>|0x28a|+0x00a|[1]:'73'
	// <0x6f9194>|0x294|+0x010:'74'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6f91a4>|0x2a4|+0x05d:'81'
	// <0>
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_start::get_look_lexeme(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
animation::mixing::animation_lexeme jump_logic_state_start::get_look_lexeme(
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
	// <0x6f8d90>|0x010|+0x00a:'90'
	// <0x6f8d9a>|0x01a|+0x007:'91'
	// <0x6f8da1>|0x021|+0x00e:'92'
	// <0x6f8daf>|0x02f|+0x02a:'93'
	// <0x6f8dd9>|0x059|+0x013:'94'
	// <0x6f8dec>|0x06c|+0x002:'95'
	// <0x6f8dee>|0x06e|+0x028:'96'
	// <0x6f8e16>|0x096|+0x011:'97'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6f8e27>|0x0a7|+0x016:'106'
	// <0x6f8e3d>|0x0bd|+0x033:'107'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6f8e70>|0x0f0|+0x07b:'114'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_start::initialize()
void jump_logic_state_start::initialize( )
{
	// FUNCTION BODY
	// <0x6f8c20>|0x010|+0x010:'119'
	// <0x6f8c30>|0x020|+0x010:'120'
	// <0x6f8c40>|0x030|+0x00a:'121'
	// <0x6f8c4a>|0x03a|+0x00a:'122'
	// <0x6f8c54>|0x044|+0x074:'123'
	// <0x6f8cc8>|0x0b8|+0x0b2:'124'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_start::finalize()
void jump_logic_state_start::finalize( )
{
	// FUNCTION BODY
	// <0x6f8a09>|0x009|+0x022:'129'
	// <0x6f8a2b>|0x02b|+0x01f:'130'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::jump_logic_state_start::on_interval_end(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum jump_logic_state_start::on_interval_end( animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0x6f8a59>|0x009|+0x030:'135'
	// <0x6f8a89>|0x039|+0x043:'136'
	// <0x6f8acc>|0x07c|+0x017:'137'
	// <0>
	// <0x6f8ae3>|0x093|+0x007:'139'
	// <0>
	// <1>
	// <2>
	// <0x6f8aea>|0x09a|+0x002:'143'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::jump_logic_state_start::on_jump_event(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum jump_logic_state_start::on_jump_event( animation::animation_callback_params& params )
{
	// CALL SITE INFO
	// <0x6f8b67> -> player_stamina& <unknown>()
	// <0x6f8ba1> -> player_stamina& <unknown>()
	// <0x6f8bbb> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6f8b09>|0x009|+0x046:'149'
	// <0>
	// <1>
	// <0x6f8b4f>|0x04f|+0x007:'152'
	// <0x6f8b56>|0x056|+0x054:'153'
	// <0x6f8baa>|0x0aa|+0x013:'154'
	// <0>
	// <1>
	// <2>
	// <0x6f8bbd>|0x0bd|+0x002:'158'
	// ******
}

} // namespace survarium
