////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_start.h"

namespace survarium {

// STATE[STUB]
// survarium::jump_logic_state_start::jump_logic_state_start(survarium::jump_logic&)
jump_logic_state_start::jump_logic_state_start( jump_logic& owner )
{
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::jump_logic_state_start::selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> jump_logic_state_start::selected_animations(
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
	// <0x6f9221>|0x000|0x000:'37'
	// <0x6f9266>|0x045|0x045:'38'
	// <0x6f928c>|0x06b|0x026:'39'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_start::get_main_lexeme(vostok::mutable_buffer&, const bool, const vostok::animation::body_part_masks_enum)
vostok::animation::mixing::animation_lexeme jump_logic_state_start::get_main_lexeme( vostok::mutable_buffer& buffer, bool is_third_view, vostok::animation::body_part_masks_enum bones_mask )
{
	// LOCALS
	// pcstr 						caption
	// vostok::fixed_vector<vostok::animation::mixing::animation_interval,2> intervals<1>
	// vostok::animation::linear_interpolator interpolator<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6f8f6d><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::fixed_vector<vostok::animation::mixing::animation_interval,2>
	// 	two_anim_intervals_type;

	// ******

	// FUNCTION BODY
	// <0x6f8f11>|0x000|0x000:'46'
	// <0x6f8f41>|0x030|0x030:'47'
	// 1
	// <0x6f8f54>|0x043|0x013:'49'
	// <0x6f8f73>|0x062|0x01f:'50'
	// <0x6f8fa1>|0x090|0x02e:'51'
	// 1
	// <0x6f8fad>|0x09c|0x00c:'53'
	// <0x6f8fc9>|0x0b8|0x01c:'54'
	// <0x6f8fea>|0x0d9|0x021:'55'
	// <0x6f903d>|0x12c|0x053:'56'
	// <0x6f9065>|0x154|0x028:'57'
	// <0x6f906f>|0x15e|0x00a:'58'
	// <0x6f9071>|0x160|0x002:'59'
	// <0x6f9099>|0x188|0x028:'60'
	// <0x6f90c1>|0x1b0|0x028:'61'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// <0x6f90cb>|0x1ba|0x00a:'71'
	// <0x6f9188>|0x277|0x0bd:'72'
	// <0x6f918a>|0x279|0x002|[1]:'73'
	// <0x6f9194>|0x283|0x00a:'74'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x6f91a4>|0x293|0x010:'81'
	// 1
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::animation_lexeme survarium::jump_logic_state_start::get_look_lexeme(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
vostok::animation::mixing::animation_lexeme jump_logic_state_start::get_look_lexeme(
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
	// <0x6f8d90>|0x000|0x000:'90'
	// <0x6f8d9a>|0x00a|0x00a:'91'
	// <0x6f8da1>|0x011|0x007:'92'
	// <0x6f8daf>|0x01f|0x00e:'93'
	// <0x6f8dd9>|0x049|0x02a:'94'
	// <0x6f8dec>|0x05c|0x013:'95'
	// <0x6f8dee>|0x05e|0x002:'96'
	// <0x6f8e16>|0x086|0x028:'97'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x6f8e27>|0x097|0x011:'106'
	// <0x6f8e3d>|0x0ad|0x016:'107'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x6f8e70>|0x0e0|0x033:'114'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_start::initialize()
void jump_logic_state_start::initialize( )
{
	// FUNCTION BODY
	// <0x6f8c20>|0x000|0x000:'119'
	// <0x6f8c30>|0x010|0x010:'120'
	// <0x6f8c40>|0x020|0x010:'121'
	// <0x6f8c4a>|0x02a|0x00a:'122'
	// <0x6f8c54>|0x034|0x00a:'123'
	// <0x6f8cc8>|0x0a8|0x074:'124'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic_state_start::finalize()
void jump_logic_state_start::finalize( )
{
	// FUNCTION BODY
	// <0x6f8a09>|0x000|0x000:'129'
	// <0x6f8a2b>|0x022|0x022:'130'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::jump_logic_state_start::on_interval_end(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum jump_logic_state_start::on_interval_end( vostok::animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// <0x6f8a59>|0x000|0x000:'135'
	// <0x6f8a89>|0x030|0x030:'136'
	// <0x6f8acc>|0x073|0x043:'137'
	// 1
	// <0x6f8ae3>|0x08a|0x017:'139'
	// 1
	// 2
	// 3
	// <0x6f8aea>|0x091|0x007:'143'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::jump_logic_state_start::on_jump_event(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum jump_logic_state_start::on_jump_event( vostok::animation::animation_callback_params& params )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e7b67 }, type_index: TypeIndex(0x9176) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e7ba1 }, type_index: TypeIndex(0x9176) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e7bbb }, type_index: TypeIndex(0x9160) })
	// ******

	// FUNCTION BODY
	// 1
	// <0x6f8b09>|0x000|0x000:'149'
	// 1
	// 2
	// <0x6f8b4f>|0x046|0x046:'152'
	// <0x6f8b56>|0x04d|0x007:'153'
	// <0x6f8baa>|0x0a1|0x054:'154'
	// 1
	// 2
	// 3
	// <0x6f8bbd>|0x0b4|0x013:'158'
	// ******
}

} // namespace survarium
