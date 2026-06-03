////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/jump_logic.h>

#include "jump_logic_base_state.h"
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/ai/fsm.h>

namespace survarium {

// STATE[100%|DONE]
move_direction_enum get_move_direction( player_input const& input )
{
	bool move_bwd_pressed;
	bool move_right_pressed;
	bool move_fwd_pressed;
	bool move_left_pressed;

	move_fwd_pressed	= ( input.actions_mask & 0x1 ) != 0;
	move_bwd_pressed	= ( input.actions_mask & 0x2 ) != 0;
	move_left_pressed	= ( input.actions_mask & 0x4 ) != 0;
	move_right_pressed	= ( input.actions_mask & 0x8 ) != 0;

	if ( move_fwd_pressed && move_bwd_pressed )
	{
		move_fwd_pressed = false;
		move_bwd_pressed = false;
	}

	if ( move_left_pressed && move_right_pressed )
	{
		move_left_pressed = false;
		move_right_pressed = false;
	}

	if ( move_fwd_pressed )
	{
		if ( move_left_pressed )
			return move_direction_fwd_left;
		else if ( move_right_pressed )
			return move_direction_fwd_right;
		else
			return move_direction_fwd;
	}
	else if ( move_bwd_pressed )
	{
		if ( move_left_pressed )
			return move_direction_back_left;
		else if ( move_right_pressed )
			return move_direction_back_right;
		else
			return move_direction_back;
	}
	else if ( move_left_pressed )
		return move_direction_left;
	else if ( move_right_pressed )
		return move_direction_right;
	else
		return move_direction_on_site;
}

// STATE[BLOCKED]: ctor body is `: m_owner(owner), m_user(0), m_logic(0),
// m_animated_object(0), m_jumping_direction(on_site), m_is_jump_from_right_leg(true)
// { initialize_logic(); }` (verified from 0x58de30 asm). Anchoring needs constructing
// a jump_logic, which runs initialize_logic -> constructs jump_logic_state_{inactive,
// start,landing}, emitting their vtables -> forces their STUB selected_animations
// (no return) to codegen -> C4716/LNK1257. Those overrides are matched only in PR #181
// (absent from this base). Unblock: match jump_logic_state_* selected_animations first.
jump_logic::jump_logic( weapon_user_animations_selector& owner ) : m_owner( owner )
{
	// FUNCTION BODY[0x58de30]: 1
	// <0x58de76>|0x046|+0x008:'76'
	// ******
}

// STATE[BLOCKED]: dtor body is `m_logic->clear_transitions(); while(fsm_state* state =
// m_logic->pop_state()) DELETE(state); DELETE(m_logic);` (verified from 0x58da30 asm,
// same shape as breath_vibration_calculator::~). Blocked with the ctor: anchoring runs
// through the state ctors -> C4716 on their STUB selected_animations. See ctor note.
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

// STATE[BLOCKED]: full body reconstructed (verified from 0x58dae0 asm, kept below as a
// comment): m_logic = NEW(ai::fsm); inactive/start/landing = NEW(jump_logic_state_*)(*this);
// add_state x3 (inactive,start,landing); add_transition(inactive,start, boost::bind<bool>(
// &true_predicate)); add_transition(start,landing, boost::bind(&jump_logic::landing_predicate,
// this)); set_initial_state(m_logic->states().front()). It COMPILES, but constructing the
// three jump_logic_state_* subclasses emits their vtables -> forces their STUB
// selected_animations (no return) -> C4716/LNK1257. Unblock by matching those overrides
// (PR #181) first, then restore this body + anchor the ctor.
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

// STATE[100%|DONE]
void jump_logic::tick( )
{
	m_logic->tick( );
}

// STATE[100%|DONE]: target params are const-qualified - the header decl now matches
// (const move_direction_enum, const bool, const jump_animation_parts) so the rich-diff
// pairs; objdiff reports 100%.
u32 get_jump_animation_index(
	const move_direction_enum		move_direction,
	const bool						jump_from_right_leg,
	const jump_animation_parts		animation_part
)
{
	enum {
		animations_per_simple_jump_direction		= 4,
		animations_per_two_legged_jump_direction	= 6,

		on_site_jump				= 0,

		forward_from_right_leg		= 4,
		forward_from_left_leg		= 10,

		forward_right_from_right_leg	= 16,
		forward_right_from_left_leg		= 22,

		right_from_right_leg		= 28,
		right_from_left_leg			= 34,

		backward_right_from_right_leg	= 40,
		backward_right_from_left_leg	= 46,

		backward_from_right_leg		= 52,
		backward_from_left_leg		= 58,

		backward_left_from_right_leg	= 64,
		backward_left_from_left_leg		= 70,

		left_from_right_leg			= 76,
		left_from_left_leg			= 82,

		left_forward_from_right_leg		= 88,
		left_forward_from_left_leg		= 94,
	};

	switch ( move_direction )
	{
	case move_direction_on_site:
		return on_site_jump + animation_part;
	case move_direction_fwd:
		return ( jump_from_right_leg ? forward_from_right_leg : forward_from_left_leg ) + animation_part;
	case move_direction_fwd_right:
		return ( jump_from_right_leg ? forward_right_from_right_leg : forward_right_from_left_leg ) + animation_part;
	case move_direction_right:
		return ( jump_from_right_leg ? right_from_right_leg : right_from_left_leg ) + animation_part;
	case move_direction_back_right:
		return ( jump_from_right_leg ? backward_right_from_right_leg : backward_right_from_left_leg ) + animation_part;
	case move_direction_back:
		return ( jump_from_right_leg ? backward_from_right_leg : backward_from_left_leg ) + animation_part;
	case move_direction_back_left:
		return ( jump_from_right_leg ? backward_left_from_right_leg : backward_left_from_left_leg ) + animation_part;
	case move_direction_left:
		return ( jump_from_right_leg ? left_from_right_leg : left_from_left_leg ) + animation_part;
	case move_direction_fwd_left:
		return ( jump_from_right_leg ? left_forward_from_right_leg : left_forward_from_left_leg ) + animation_part;
	default:
		NODEFAULT( );
	}
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
// m_jumping_direction * 3, is_third_view )` (verified against target asm @0x58dee0,
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
// m_jumping_direction * 3 + 2, is_third_view )` (target asm @0x58de90). Same
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

// STATE[45.13%|PARTIAL]: body is the single statement the carcass dictates and the
// control flow matches; residual is the ai-fsm inline-vs-call wall: TARGET out-of-lines
// fsm::states() (folds to 0x03f210, delinker-misnamed finalize_impl) + the front()/
// operator[] accessor, while our in-class accessors fold to direct field loads
// (`mov ecx,[eax+8]`), shifting the whole frame (sub esp,0Ch + [ebp-4]/[ebp-8] temps the
// target lacks). Confirmed: fsm::states() exists out-of-line in TARGET, absent in BASE.
// This is the permitted LTCG call-boundary class; not source-steerable (no cast/loop here).
// objdiff measure 45.13% (report.json - the 7/26 = 26.9% diff-VIEW footer under-counts vs
// the authoritative report measure). See jump_logic.md.
void jump_logic::deactivate( )
{
	m_logic->set_initial_state( m_logic->states( ).front( ) );

	// FUNCTION BODY[0x58d850]: 1
	// <0x58d857>|0x007|+0x01c:'192'
	// ******
}

// STATE[83.61%|PARTIAL]: the steerable cast-temp residual is now FIXED - binding the
// static_cast<jump_logic_base_state*> result to a named local `state` before the call
// reproduces the target's "materialize cast into its own slot, then push user" order
// (mov [slot],edx; push user; mov ecx,[slot]; ...). objdiff measure 83.61% (report.json).
// The remaining residual is the ai-fsm inline-vs-call WALL (legit LTCG call-boundary):
// the TARGET out-of-lines fsm::states() (delinker-misnamed finalize_impl) + the front()/
// operator[] accessor (two `call`s), while our in-class accessors fold inline to direct
// field loads, costing one extra stack slot (sub esp,10h vs 0Ch) and shifting the slot
// numbering. Confirmed out-of-line in TARGET, absent in BASE - not source-steerable here.
// See jump_logic.md.
void jump_logic::set_user( base_player& user )
{
	m_user = &user;

	for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next )
	{
		jump_logic_base_state* state = static_cast< jump_logic_base_state* >( i );
		state->set_user( user );
	}

	// FUNCTION BODY[0x58d7f0]: 4
	// <0x58d7f9>|0x009|+0x009:'197'
	// <0>
	// <0x58d802>|0x012|+0x024|[1]:'199'
	// <0x58d826>|0x036|+0x019:'200'
	// ******
}

// STATE[None|DONE]: body is `mov al,1; ret` (8/8 bytes equal by RVA diff @0x57d4f0);
// /OPT:ICF folds this `return true;` into a shared fold body, so objdiff reports
// it unpaired (None) - byte-correct, not separately scorable.
bool jump_logic::landing_predicate( ) const
{
	return true;
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

// STATE[100%|DONE]
float jump_logic::look_time_factor( ) const
{
	return m_owner.look_time_factor( );
}

// STATE[100%|DONE]
bool jump_logic::does_need_land_and_run( ) const
{
	move_direction_enum landing_direction = get_move_direction( m_user->input( ) );

	switch ( m_jumping_direction )
	{
	case move_direction_on_site:
		return false;
	case move_direction_fwd:
		return landing_direction == move_direction_fwd || landing_direction == move_direction_fwd_left || landing_direction == move_direction_fwd_right;
	case move_direction_fwd_right:
		return landing_direction == move_direction_fwd_right || landing_direction == move_direction_fwd || landing_direction == move_direction_right;
	case move_direction_right:
		return landing_direction == move_direction_right || landing_direction == move_direction_fwd_right || landing_direction == move_direction_back_right;
	case move_direction_back_right:
		return landing_direction == move_direction_back_right || landing_direction == move_direction_right || landing_direction == move_direction_back;
	case move_direction_back:
		return landing_direction == move_direction_back || landing_direction == move_direction_back_right || landing_direction == move_direction_back_left;
	case move_direction_back_left:
		return landing_direction == move_direction_back_left || landing_direction == move_direction_back || landing_direction == move_direction_left;
	case move_direction_left:
		return landing_direction == move_direction_left || landing_direction == move_direction_back_left || landing_direction == move_direction_fwd_left;
	case move_direction_fwd_left:
		return landing_direction == move_direction_fwd_left || landing_direction == move_direction_left || landing_direction == move_direction_fwd;
	default:
		NODEFAULT( );
	}
}

// STATE[100%|DONE]
bool jump_logic::is_jump_finished( ) const
{
	return static_cast< jump_logic_base_state* >( m_logic->current_state( ) )->is_jump_finished( );
}

} // namespace survarium
