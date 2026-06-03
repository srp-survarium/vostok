////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/jump_logic.h>

#include "jump_logic_base_state.h"
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/base_player.h>
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

// STATE[STUB]
void jump_logic::tick( )
{
	// FUNCTION BODY[0x58dac0]: 1
	// <0x58dac7>|0x007|+0x00b:'109'
	// ******
}

// STATE[100%|DONE]
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

// STATE[STUB]
resources::managed_resource_ptr jump_logic::get_animation(
	const jump_animation_parts		arg_0 /* jump_animation_parts anim_part */,
	const bool						is_third_view
) const
{
	// FUNCTION BODY[0x58df30]: 1
	// <0x58df39>|0x009|+0x079:'162'
	// ******
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

// STATE[STUB]
resources::managed_resource_ptr jump_logic::get_move_animation( const bool is_third_view ) const
{
	// FUNCTION BODY[0x58dee0]: 1
	// <0x58dee9>|0x009|+0x03d:'172'
	// ******
}

// STATE[STUB]
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

// STATE[STUB]
void jump_logic::activate( )
{
	// CALL SITE INFO
	// <0x58d89c> -> player_input const& < unknown >() const
	// ******

	// FUNCTION BODY[0x58d880]: 2
	// <0x58d889>|0x009|+0x024:'186'
	// <0x58d8ad>|0x02d|+0x01c:'187'
	// ******
}

// STATE[26.9%|PARTIAL]: body is the single statement the carcass dictates and the
// control flow matches; residual is the ai-fsm inline-vs-call wall: TARGET out-of-lines
// fsm::states() (folds to 0x03f210, delinker-misnamed finalize_impl) + the front()/
// operator[] accessor, while our in-class accessors fold to direct field loads
// (`mov ecx,[eax+8]`), shifting the whole frame (sub esp,0Ch + [ebp-4]/[ebp-8] temps the
// target lacks). Confirmed: fsm::states() exists out-of-line in TARGET, absent in BASE.
// This part is the permitted LTCG call-boundary class; blocked on the ai fsm type for the
// rest. (reviewer: re-measured 7/26 = 26.9% on the committed index, not the banked 45.13%.)
// See jump_logic.md.
void jump_logic::deactivate( )
{
	m_logic->set_initial_state( m_logic->states( ).front( ) );

	// FUNCTION BODY[0x58d850]: 1
	// <0x58d857>|0x007|+0x01c:'192'
	// ******
}

// STATE[41.9%|PARTIAL]: body + loop control flow match the carcass. TWO residuals:
// (1) the ai-fsm inline-vs-call wall - TARGET out-of-lines fsm::states()/front()
//     (states() folds to 0x03f210; the front()/operator[] accessor is the other call),
//     base folds them inline -> the loop-head `this` slot shifts ([ebp-8] vs [ebp-0Ch]).
//     Confirmed out-of-line in TARGET, absent in BASE - the permitted LTCG class.
// (2) claude@NOTE: a SOURCE-SHAPE residual in the loop BODY that is NOT the accessor wall:
//     the TARGET materializes the static_cast<jump_logic_base_state*> result into its OWN
//     slot ([ebp-0Ch]) BEFORE pushing the `user` arg (mov [slot],edx; push user; mov ecx,
//     [slot]; ...), whereas our base pushes user first then derefs `i` directly with no
//     cast-temp slot. That extra cast temp + statement order may be steerable once the
//     fsm type lands (e.g. binding the cast result to a named local before the call), and
//     is worth a re-match attempt - it is distinct from the inline-vs-call wall.
// (reviewer: re-measured 18/43 = 41.9% on the committed index, not the banked 76.55%.)
// Blocked on the ai fsm type. See jump_logic.md.
void jump_logic::set_user( base_player& user )
{
	m_user = &user;

	for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next )
		static_cast< jump_logic_base_state* >( i )->set_user( user );

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

// STATE[STUB]
std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > jump_logic::selected_animations(
	mutable_buffer&							buffer,
	weapon_animation_parameters const&		weapon_parameters,
	const bool								is_third_view
) const
{
	// CALL SITE INFO
	// <0x58da16> -> std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > < unknown >( mutable_buffer&, const bool, fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > const&, weapon_animation_parameters const& )
	// ******

	// FUNCTION BODY[0x58d9d0]: 1
	// <0x58d9d9>|0x009|+0x042:'210'
	// ******
}

// STATE[STUB]
float jump_logic::look_time_factor( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x58d9b0]: 1
	// <0x58d9b7>|0x007|+0x00a:'215'
	// ******
}

// STATE[63.7%|INPROGRESS]: body + per-case ORs are byte-identical, but the match is NOT
// 100% (reviewer re-measured: 102/160 = 63.7% vs the committed base/target index; the
// prior 100% claim did not hold on this build). The ONLY diff is a systematic extra join
// block: every leaf does `jmp .17` (base) vs `jmp .18` (target) - the TARGET has ONE more
// trailing statement/block than the base. Target structure has 13 stmts ending at L259
// (0x188, +0x28) - a statement AFTER the last case that the return-per-case base lacks.
// NEXT STEP (source-steerable, same class as movement_animation_index None->100%): the
// target almost certainly assigned a result and fell through to one shared `return`, i.e.
// `bool result; switch(...){ case X: result = ...; break; ... } return result;` (or a
// trailing `return false;` after the switch), not a direct `return` in each case. Restore
// the result-temp + single tail return so the leaves share one epilogue (.18). See jump_logic.md.
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

	// FUNCTION BODY[0x58d640]: 13   (target structure: 13 stmts, ends L259 @0x188 - the
	// extra trailing block absent from the base's return-per-case shape)
	// <0x58d649>|0x009|+0x021:'220'
	// <0x58d66a>|0x02a|+0x013:'221'
	// <0x58d67d>|0x03d|+0x007:'223'	case on_site: return false
	// <0x58d684>|0x044|+0x02a:'227'	case fwd
	// <0x58d6ae>|0x06e|+0x02a:'231'	case fwd_right
	// <0x58d6d8>|0x098|+0x02a:'235'	case right
	// <0x58d702>|0x0c2|+0x02a:'239'	case back_right
	// <0x58d72c>|0x0ec|+0x027:'243'	case back
	// <0x58d753>|0x113|+0x027:'247'	case back_left
	// <0x58d77a>|0x13a|+0x027:'251'	case left
	// <0x58d7a1>|0x161|+0x027:'255'	case fwd_left
	// <0x58d7??>|0x188|+0x028:'259'	<- trailing stmt the base is MISSING (shared epilogue)
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