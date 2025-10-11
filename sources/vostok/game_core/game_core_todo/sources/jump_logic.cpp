////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic.h"

namespace survarium {

// STATE[STUB]
// survarium::move_direction_enum survarium::get_move_direction(survarium::player_input const&)
move_direction_enum get_move_direction( player_input const& input )
{
	// LOCALS
	// bool 						move_bwd_pressed
	// bool 						move_right_pressed
	// bool 						move_fwd_pressed
	// bool 						move_left_pressed
	// ******

	// FUNCTION BODY
	// <0x58d504>|0x000|0x000:'25'
	// <0x58d516>|0x012|0x012:'26'
	// <0x58d528>|0x024|0x012:'27'
	// <0x58d53a>|0x036|0x012:'28'
	// 1
	// <0x58d54c>|0x048|0x012:'30'
	// 1
	// <0x58d55c>|0x058|0x010:'32'
	// <0x58d560>|0x05c|0x004:'33'
	// 1
	// 2
	// <0x58d564>|0x060|0x004:'36'
	// 1
	// <0x58d574>|0x070|0x010:'38'
	// <0x58d578>|0x074|0x004:'39'
	// 1
	// 2
	// <0x58d57c>|0x078|0x004:'42'
	// 1
	// <0x58d584>|0x080|0x008:'44'
	// <0x58d58c>|0x088|0x008:'45'
	// <0x58d595>|0x091|0x009:'46'
	// <0x58d59d>|0x099|0x008:'47'
	// <0x58d5a4>|0x0a0|0x007:'48'
	// <0x58d5a6>|0x0a2|0x002:'49'
	// 1
	// <0x58d5af>|0x0ab|0x009:'51'
	// 1
	// <0x58d5b7>|0x0b3|0x008:'53'
	// <0x58d5bf>|0x0bb|0x008:'54'
	// <0x58d5c8>|0x0c4|0x009:'55'
	// <0x58d5d0>|0x0cc|0x008:'56'
	// <0x58d5d7>|0x0d3|0x007:'57'
	// <0x58d5d9>|0x0d5|0x002:'58'
	// 1
	// <0x58d5e2>|0x0de|0x009:'60'
	// <0x58d5ea>|0x0e6|0x008:'61'
	// <0x58d5f3>|0x0ef|0x009:'62'
	// <0x58d5fb>|0x0f7|0x008:'63'
	// <0x58d602>|0x0fe|0x007:'64'
	// <0x58d604>|0x100|0x002:'65'
	// ******
}

// STATE[STUB]
// survarium::jump_logic::jump_logic(survarium::weapon_user_animations_selector&)
jump_logic::jump_logic( weapon_user_animations_selector& owner )
{
	// FUNCTION BODY
	// <0x58de76>|0x000|0x000:'76'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic::~jump_logic()
void jump_logic::~jump_logic( )
{
	// LOCALS
	// vostok::ai::fsm_state* 		state<1>
	// ******

	// FUNCTION BODY
	// <0x58da39>|0x000|0x000:'81'
	// <0x58da44>|0x00b|0x00b|[1]:'82'
	// <0x58da58>|0x01f|0x014:'83'
	// <0x58da80>|0x047|0x028:'84'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic::initialize_logic()
void jump_logic::initialize_logic( )
{
	// LOCALS
	// jump_logic_base_state* 		start
	// jump_logic_base_state* 		landing
	// jump_logic_base_state* 		inactive
	// ******

	// FUNCTION BODY
	// <0x58daf0>|0x000|0x000:'94'
	// <0x58db4a>|0x05a|0x05a:'95'
	// <0x58dbb1>|0x0c1|0x067:'96'
	// <0x58dc18>|0x128|0x067:'97'
	// 1
	// <0x58dc7f>|0x18f|0x067:'99'
	// <0x58dc91>|0x1a1|0x012:'100'
	// <0x58dca3>|0x1b3|0x012:'101'
	// <0x58dcb5>|0x1c5|0x012:'102'
	// <0x58dd58>|0x268|0x0a3:'103'
	// <0x58de02>|0x312|0x0aa:'104'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic::tick()
void jump_logic::tick( )
{
	// FUNCTION BODY
	// <0x58dac7>|0x000|0x000:'109'
	// ******
}

// STATE[STUB]
// unsigned int survarium::get_jump_animation_index(const survarium::move_direction_enum, const bool, const survarium::jump_animation_parts)
u32 get_jump_animation_index( move_direction_enum move_direction, bool jump_from_right_leg, jump_animation_parts animation_part )
{
	// CONSTANTS
	// const get_jump_animation_index::__l2::<unnamed-tag> on_site_jump = 0;
	// const get_jump_animation_index::__l2::<unnamed-tag> right_from_left_leg = 34;
	// const get_jump_animation_index::__l2::<unnamed-tag> backward_from_left_leg = 58;
	// const get_jump_animation_index::__l2::<unnamed-tag> backward_left_from_right_leg = 64;
	// const get_jump_animation_index::__l2::<unnamed-tag> left_from_left_leg = 82;
	// const get_jump_animation_index::__l2::<unnamed-tag> forward_from_right_leg = 4;
	// const get_jump_animation_index::__l2::<unnamed-tag> backward_left_from_left_leg = 70;
	// const get_jump_animation_index::__l2::<unnamed-tag> left_forward_from_right_leg = 88;
	// const get_jump_animation_index::__l2::<unnamed-tag> left_from_right_leg = 76;
	// const get_jump_animation_index::__l2::<unnamed-tag> left_forward_from_left_leg = 94;
	// const get_jump_animation_index::__l2::<unnamed-tag> backward_from_right_leg = 52;
	// const get_jump_animation_index::__l2::<unnamed-tag> right_from_right_leg = 28;
	// const get_jump_animation_index::__l2::<unnamed-tag> animations_per_two_legged_jump_direction = 6;
	// const get_jump_animation_index::__l2::<unnamed-tag> backward_right_from_right_leg = 40;
	// const get_jump_animation_index::__l2::<unnamed-tag> forward_right_from_left_leg = 22;
	// const get_jump_animation_index::__l2::<unnamed-tag> backward_right_from_left_leg = 46;
	// const get_jump_animation_index::__l2::<unnamed-tag> animations_per_simple_jump_direction = 4;
	// const get_jump_animation_index::__l2::<unnamed-tag> forward_right_from_right_leg = 16;
	// const get_jump_animation_index::__l2::<unnamed-tag> forward_from_left_leg = 10;
	// ******

	// STATICS
	// static <NoType> 				 = <0xcd33c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc294 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc29c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc2b2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc2c5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc2d8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc2eb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc2fe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc311 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xbc324 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	return 0;
	// FUNCTION BODY
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
	// 15
	// 16
	// 17
	// 18
	// 19
	// 20
	// 21
	// 22
	// <0xcd284>|0x000|0x000:'136'
	// 1
	// <0xcd294>|0x010|0x010:'138'
	// 1
	// <0xcd29c>|0x018|0x008:'140'
	// 1
	// <0xcd2b2>|0x02e|0x016:'142'
	// 1
	// <0xcd2c5>|0x041|0x013:'144'
	// 1
	// <0xcd2d8>|0x054|0x013:'146'
	// 1
	// <0xcd2eb>|0x067|0x013:'148'
	// 1
	// <0xcd2fe>|0x07a|0x013:'150'
	// 1
	// <0xcd311>|0x08d|0x013:'152'
	// 1
	// <0xcd324>|0x0a0|0x013:'154'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> survarium::jump_logic::get_animation(const survarium::jump_animation_parts, const bool) const
vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> jump_logic::get_animation( jump_animation_parts anim_part, bool is_third_view ) const
{
	// FUNCTION BODY
	// <0x58df39>|0x000|0x000:'162'
	// ******
}

// STATE[STUB]
// char const* survarium::jump_logic::get_animation_caption(const survarium::jump_animation_parts) const
pcstr jump_logic::get_animation_caption( jump_animation_parts anim_part ) const
{
	return NULL;
	// FUNCTION BODY
	// <0x58d939>|0x000|0x000:'167'
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> survarium::jump_logic::get_move_animation(const bool) const
vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> jump_logic::get_move_animation( bool is_third_view ) const
{
	// FUNCTION BODY
	// <0x58dee9>|0x000|0x000:'172'
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> survarium::jump_logic::get_move_look_animation(const bool) const
vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> jump_logic::get_move_look_animation( bool is_third_view ) const
{
	// FUNCTION BODY
	// <0x58de99>|0x000|0x000:'177'
	// ******
}

// STATE[STUB]
// char const* survarium::jump_logic::get_move_look_caption() const
pcstr jump_logic::get_move_look_caption( ) const
{
	return NULL;
	// FUNCTION BODY
	// <0x58d8d9>|0x000|0x000:'181'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic::activate()
void jump_logic::activate( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c89c }, type_index: TypeIndex(0x916f) })
	// ******

	// FUNCTION BODY
	// <0x58d889>|0x000|0x000:'186'
	// <0x58d8ad>|0x024|0x024:'187'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic::deactivate()
void jump_logic::deactivate( )
{
	// FUNCTION BODY
	// <0x58d857>|0x000|0x000:'192'
	// ******
}

// STATE[STUB]
// void survarium::jump_logic::set_user(survarium::base_player&)
void jump_logic::set_user( base_player& user )
{
	// LOCALS
	// vostok::ai::fsm_state* 		i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c83b }, type_index: TypeIndex(0x4b406) })
	// ******

	// FUNCTION BODY
	// <0x58d7f9>|0x000|0x000:'197'
	// 1
	// <0x58d802>|0x009|0x009|[1]:'199'
	// <0x58d826>|0x02d|0x024:'200'
	// ******
}

// STATE[STUB]
// bool survarium::jump_logic::landing_predicate() const
bool jump_logic::landing_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x58d4f7>|0x000|0x000:'205'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::jump_logic::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> jump_logic::selected_animations( vostok::mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57ca16 }, type_index: TypeIndex(0x4b408) })
	// ******

	// FUNCTION BODY
	// <0x58d9d9>|0x000|0x000:'210'
	// ******
}

// STATE[STUB]
// float survarium::jump_logic::look_time_factor() const
float jump_logic::look_time_factor( ) const
{
	return 0.0f;
	// FUNCTION BODY
	// <0x58d9b7>|0x000|0x000:'215'
	// ******
}

// STATE[STUB]
// bool survarium::jump_logic::does_need_land_and_run() const
bool jump_logic::does_need_land_and_run( ) const
{
	// LOCALS
	// move_direction_enum 			landing_direction
	// ******

	// STATICS
	// static <NoType> 				 = <0x58d7cc>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c67d }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c684 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c6ae }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c6d8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c702 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c72c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c753 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c77a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c7a1 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57c65c }, type_index: TypeIndex(0x916f) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x58d649>|0x000|0x000:'220'
	// <0x58d66a>|0x021|0x021:'221'
	// 1
	// <0x58d67d>|0x034|0x013:'223'
	// 1
	// 2
	// 3
	// <0x58d684>|0x03b|0x007:'227'
	// 1
	// 2
	// 3
	// <0x58d6ae>|0x065|0x02a:'231'
	// 1
	// 2
	// 3
	// <0x58d6d8>|0x08f|0x02a:'235'
	// 1
	// 2
	// 3
	// <0x58d702>|0x0b9|0x02a:'239'
	// 1
	// 2
	// 3
	// <0x58d72c>|0x0e3|0x02a:'243'
	// 1
	// 2
	// 3
	// <0x58d753>|0x10a|0x027:'247'
	// 1
	// 2
	// 3
	// <0x58d77a>|0x131|0x027:'251'
	// 1
	// 2
	// 3
	// <0x58d7a1>|0x158|0x027:'255'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// bool survarium::jump_logic::is_jump_finished() const
bool jump_logic::is_jump_finished( ) const
{
	return false;
	// FUNCTION BODY
	// <0x58d619>|0x000|0x000:'263'
	// ******
}

} // namespace survarium
