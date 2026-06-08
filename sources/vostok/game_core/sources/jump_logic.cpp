////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/jump_logic.h>

#include <vostok/game_core/player_input.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/ai/fsm.h>
#include "jump_logic_base_state.h"

namespace survarium {

// STATE[STUB]
move_direction_enum get_move_direction( player_input const& input )
{
	// LOCALS
	// bool 							move_bwd_pressed
	// bool 							move_right_pressed
	// bool 							move_fwd_pressed
	// bool 							move_left_pressed
	// ******

	// FUNCTION BODY[0x58d500]: 41
	// <0x58d504>|0x004|+0x012:'25'
	// <0x58d516>|0x016|+0x012:'26'
	// <0x58d528>|0x028|+0x012:'27'
	// <0x58d53a>|0x03a|+0x012:'28'
	// <0>
	// <0x58d54c>|0x04c|+0x010:'30'
	// <0>
	// <0x58d55c>|0x05c|+0x004:'32'
	// <0x58d560>|0x060|+0x004:'33'
	// <0>
	// <1>
	// <0x58d564>|0x064|+0x010:'36'
	// <0>
	// <0x58d574>|0x074|+0x004:'38'
	// <0x58d578>|0x078|+0x004:'39'
	// <0>
	// <1>
	// <0x58d57c>|0x07c|+0x008:'42'
	// <0>
	// <0x58d584>|0x084|+0x008:'44'
	// <0x58d58c>|0x08c|+0x009:'45'
	// <0x58d595>|0x095|+0x008:'46'
	// <0x58d59d>|0x09d|+0x007:'47'
	// <0x58d5a4>|0x0a4|+0x002:'48'
	// <0x58d5a6>|0x0a6|+0x009:'49'
	// <0>
	// <0x58d5af>|0x0af|+0x008:'51'
	// <0>
	// <0x58d5b7>|0x0b7|+0x008:'53'
	// <0x58d5bf>|0x0bf|+0x009:'54'
	// <0x58d5c8>|0x0c8|+0x008:'55'
	// <0x58d5d0>|0x0d0|+0x007:'56'
	// <0x58d5d7>|0x0d7|+0x002:'57'
	// <0x58d5d9>|0x0d9|+0x009:'58'
	// <0>
	// <0x58d5e2>|0x0e2|+0x008:'60'
	// <0x58d5ea>|0x0ea|+0x009:'61'
	// <0x58d5f3>|0x0f3|+0x008:'62'
	// <0x58d5fb>|0x0fb|+0x007:'63'
	// <0x58d602>|0x102|+0x002:'64'
	// <0x58d604>|0x104|+0x002:'65'
	// ******

	// claude@NOTE: placeholder return so callers (activate) link; body still STUB.
	return move_direction_on_site;
}

// STATE[STUB]
jump_logic::jump_logic( weapon_user_animations_selector& owner ) : m_owner( owner )
{
	// FUNCTION BODY[0x58de30]: 1
	// <0x58de76>|0x046|+0x008:'76'
	// ******
}

// STATE[STUB]
 jump_logic::~jump_logic( )
{
	// LOCALS
	// ai::fsm_state* 					state<1>
	// ******

	// FUNCTION BODY[0x58da30]: 4
	// <0x58da39>|0x009|+0x00b:'81'
	// <0x58da44>|0x014|+0x014|[1]:'82'
	// <0x58da58>|0x028|+0x028:'83'
	// <0x58da80>|0x050|+0x029:'84'
	// ******
}

// STATE[STUB]
void jump_logic::initialize_logic( )
{
	// LOCALS
	// jump_logic_base_state* 			start
	// jump_logic_base_state* 			landing
	// jump_logic_base_state* 			inactive
	// ******

	// FUNCTION BODY[0x58dae0]: 11
	// <0x58daf0>|0x010|+0x05a:'94'
	// <0x58db4a>|0x06a|+0x067:'95'
	// <0x58dbb1>|0x0d1|+0x067:'96'
	// <0x58dc18>|0x138|+0x067:'97'
	// <0>
	// <0x58dc7f>|0x19f|+0x012:'99'
	// <0x58dc91>|0x1b1|+0x012:'100'
	// <0x58dca3>|0x1c3|+0x012:'101'
	// <0x58dcb5>|0x1d5|+0x0a3:'102'
	// <0x58dd58>|0x278|+0x0aa:'103'
	// <0x58de02>|0x322|+0x022:'104'
	// ******
}

// STATE[STUB]
void jump_logic::tick( )
{
	// FUNCTION BODY[0x58dac0]: 1
	// <0x58dac7>|0x007|+0x00b:'109'
	// ******
}

// STATE[STUB]
u32 get_jump_animation_index(
	const move_direction_enum		arg_0 /* move_direction_enum move_direction */,
	const bool						jump_from_right_leg,
	const jump_animation_parts		arg_2 /* jump_animation_parts animation_part */
)
{
	// CONSTANTS
	// const get_jump_animation_index::__l2::< unnamed-tag > on_site_jump = 0;
	// const get_jump_animation_index::__l2::< unnamed-tag > right_from_left_leg = 34;
	// const get_jump_animation_index::__l2::< unnamed-tag > backward_from_left_leg = 58;
	// const get_jump_animation_index::__l2::< unnamed-tag > backward_left_from_right_leg = 64;
	// const get_jump_animation_index::__l2::< unnamed-tag > left_from_left_leg = 82;
	// const get_jump_animation_index::__l2::< unnamed-tag > forward_from_right_leg = 4;
	// const get_jump_animation_index::__l2::< unnamed-tag > backward_left_from_left_leg = 70;
	// const get_jump_animation_index::__l2::< unnamed-tag > left_forward_from_right_leg = 88;
	// const get_jump_animation_index::__l2::< unnamed-tag > left_from_right_leg = 76;
	// const get_jump_animation_index::__l2::< unnamed-tag > left_forward_from_left_leg = 94;
	// const get_jump_animation_index::__l2::< unnamed-tag > backward_from_right_leg = 52;
	// const get_jump_animation_index::__l2::< unnamed-tag > right_from_right_leg = 28;
	// const get_jump_animation_index::__l2::< unnamed-tag > animations_per_two_legged_jump_direction = 6;
	// const get_jump_animation_index::__l2::< unnamed-tag > backward_right_from_right_leg = 40;
	// const get_jump_animation_index::__l2::< unnamed-tag > forward_right_from_left_leg = 22;
	// const get_jump_animation_index::__l2::< unnamed-tag > backward_right_from_left_leg = 46;
	// const get_jump_animation_index::__l2::< unnamed-tag > animations_per_simple_jump_direction = 4;
	// const get_jump_animation_index::__l2::< unnamed-tag > forward_right_from_right_leg = 16;
	// const get_jump_animation_index::__l2::< unnamed-tag > forward_from_left_leg = 10;
	// ******

	// STATICS
	// static < NoType > 				 = <0xcd33c>;
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

	// FUNCTION BODY[0xcd280]: 44
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
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <0xcd284>|0x004|+0x010:'136'
	// <0>
	// <0xcd294>|0x014|+0x008:'138'
	// <0>
	// <0xcd29c>|0x01c|+0x016:'140'
	// <0>
	// <0xcd2b2>|0x032|+0x013:'142'
	// <0>
	// <0xcd2c5>|0x045|+0x013:'144'
	// <0>
	// <0xcd2d8>|0x058|+0x013:'146'
	// <0>
	// <0xcd2eb>|0x06b|+0x013:'148'
	// <0>
	// <0xcd2fe>|0x07e|+0x013:'150'
	// <0>
	// <0xcd311>|0x091|+0x013:'152'
	// <0>
	// <0xcd324>|0x0a4|+0x013:'154'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[100%|DONE]
resources::managed_resource_ptr jump_logic::get_animation( jump_animation_parts anim_part, bool is_third_view ) const
{
	return m_owner.animations().get_jump_animation(
		get_jump_animation_index( get_jump_direction( ), is_jump_from_right_leg( ), anim_part ),
		is_third_view
	);
}

// STATE[STUB]
pcstr jump_logic::get_animation_caption(
	const jump_animation_parts		arg_0 /* jump_animation_parts anim_part */
) const
{
	return NULL;

	// FUNCTION BODY[0x58d930]: 1
	// <0x58d939>|0x009|+0x063:'167'
	// ******
}

// STATE[STUB]: body is `return m_owner.animations().get_stand_animation( false,
// m_jumping_direction * 3, is_third_view )` (verified against target asm @0x57dee0,
// 1 empty_stub ASSERT for animations() operator*), but get_stand_animation has no
// definition in our tree (weapon_user_animations_container.cpp absent) so anchoring
// this body fails LTCG with LNK1257 (unresolved). Unblock once that symbol exists.
resources::managed_resource_ptr jump_logic::get_move_animation( const bool is_third_view ) const
{
	// FUNCTION BODY[0x58dee0]: 1
	// <0x58dee9>|0x009|+0x03d:'172'
	// ******
}

// STATE[STUB]: body is `return m_owner.animations().get_stand_animation( false,
// m_jumping_direction * 3 + 2, is_third_view )` (target asm @0x57de90). Same
// get_stand_animation undefined-symbol blocker as get_move_animation.
resources::managed_resource_ptr jump_logic::get_move_look_animation( const bool is_third_view ) const
{
	// FUNCTION BODY[0x58de90]: 1
	// <0x58de99>|0x009|+0x040:'177'
	// ******
}

// STATE[STUB]
pcstr jump_logic::get_move_look_caption( ) const
{
	return NULL;

	// FUNCTION BODY[0x58d8d0]: 1
	// <0x58d8d9>|0x009|+0x048:'181'
	// ******
}

// STATE[100%|DONE]
void jump_logic::activate( )
{
	m_jumping_direction			= get_move_direction( m_user->input( ) );
	m_is_jump_from_right_leg	= !m_owner.is_right_leg_supporting( );
}

// STATE[STUB]
void jump_logic::deactivate( )
{
	// FUNCTION BODY[0x58d850]: 1
	// <0x58d857>|0x007|+0x01c:'192'
	// ******
}

// STATE[STUB]
void jump_logic::set_user( base_player& user )
{
	// LOCALS
	// ai::fsm_state* 					i<1>
	// ******

	// CALL SITE INFO
	// <0x58d83b> -> void < unknown >( base_player& )
	// ******

	// FUNCTION BODY[0x58d7f0]: 4
	// <0x58d7f9>|0x009|+0x009:'197'
	// <0>
	// <0x58d802>|0x012|+0x024|[1]:'199'
	// <0x58d826>|0x036|+0x019:'200'
	// ******
}

// STATE[STUB]
bool jump_logic::landing_predicate( ) const
{
	return false;

	// FUNCTION BODY[0x58d4f0]: 1
	// <0x58d4f7>|0x007|+0x002:'205'
	// ******
}

// STATE[100%|DONE]
std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > jump_logic::selected_animations(
	mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view
) const
{
	return static_cast< jump_logic_base_state* >( m_logic->current_state( ) )->selected_animations(
		buffer, is_third_view, m_owner.look_time_calculator( ), weapon_parameters
	);
}

// STATE[STUB]
float jump_logic::look_time_factor( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x58d9b0]: 1
	// <0x58d9b7>|0x007|+0x00a:'215'
	// ******
}

// STATE[STUB]
bool jump_logic::does_need_land_and_run( ) const
{
	// LOCALS
	// move_direction_enum 				landing_direction
	// ******

	// STATICS
	// static < NoType > 				 = <0x58d7cc>;
	// ******

	// CALL SITE INFO
	// <0x58d65c> -> player_input const& < unknown >() const
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
	// ******

	return false;

	// FUNCTION BODY[0x58d640]: 39
	// <0x58d649>|0x009|+0x021:'220'
	// <0x58d66a>|0x02a|+0x013:'221'
	// <0>
	// <0x58d67d>|0x03d|+0x007:'223'
	// <0>
	// <1>
	// <2>
	// <0x58d684>|0x044|+0x02a:'227'
	// <0>
	// <1>
	// <2>
	// <0x58d6ae>|0x06e|+0x02a:'231'
	// <0>
	// <1>
	// <2>
	// <0x58d6d8>|0x098|+0x02a:'235'
	// <0>
	// <1>
	// <2>
	// <0x58d702>|0x0c2|+0x02a:'239'
	// <0>
	// <1>
	// <2>
	// <0x58d72c>|0x0ec|+0x027:'243'
	// <0>
	// <1>
	// <2>
	// <0x58d753>|0x113|+0x027:'247'
	// <0>
	// <1>
	// <2>
	// <0x58d77a>|0x13a|+0x027:'251'
	// <0>
	// <1>
	// <2>
	// <0x58d7a1>|0x161|+0x027:'255'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
bool jump_logic::is_jump_finished( ) const
{
	return false;

	// FUNCTION BODY[0x58d610]: 1
	// <0x58d619>|0x009|+0x018:'263'
	// ******
}

} // namespace survarium