////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/breath_vibration_calculator.h>
#include <vostok/game_core/breath_state.h>
#include <vostok/game_core/breath_holding_params.h>
#include <vostok/game_core/base_player.h>
#include <vostok/console_command.h>

namespace survarium {

// STATE[None|DONE]: `dynamic initializer for 's_enable_breath_vibration_cc'`. The
// dynamic initializer is the static construction of s_enable_breath_vibration_cc
// below. Like dispersion_calculator's s_dispersion_enabled_cc it is objdiff-
// unscorable (None) - the file-static cc_bool's init/atexit thunks are LTCG/ICF
// folded so no standalone symbol re-attaches. asm: push 1 (serializable), value
// ref, "breath_vibration_enable", command_type_user_specific (eax=1), default
// execution_filter (ecx=0).
static bool s_enable_breath_vibration_value = true;
static console_commands::cc_bool s_enable_breath_vibration_cc( "breath_vibration_enable", s_enable_breath_vibration_value, true, console_commands::command_type_user_specific );
// STATE[100%|DONE]
breath_vibration_calculator::breath_vibration_calculator( )
	:	m_user						( 0 ),
		m_params					( 0 ),
		m_last_time_in_ms			( 0 ),
		m_current_multiplier		( 0.0f ),
		m_target_multiplier			( 0.0f ),
		m_vertical_value			( 0.0f ),
		m_horizontal_value			( 0.0f ),
		m_character_multiplier		( 0.0f ),
		m_breath_holding_reserve	( 0.0f ),
		m_is_breath_holded			( false )
{
	initialize_logic( );
}

// STATE[76.80%|PARTIAL]: source SHAPE now matched - bracing the for body and chaining
// `m_current_multiplier = m_target_multiplier = ...->get_multiplier()` into one statement
// removed both structure divergences (the L37 back-edge row and the `ONLY base` separate
// assignment are gone; quantity-diffs dropped from 5 to blank-line gaps only). The flat
// 76.80% is the residual `m_logic.states().front()` inline-vs-out-of-line codegen (all four
// SIZE rows), blocked on the ai::fsm type - NOT source-steerable. See structure/breath_vibration-set_breath_holding_params.md
void breath_vibration_calculator::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;

	for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )
	{
		static_cast< breath_state* >( it )->set_breath_holding_params( params );
	}

	if ( m_params )
	{
		m_breath_holding_reserve	= m_params->max_breath_holding_time;
		m_logic.set_initial_state( m_logic.states( ).front( ) );
		m_current_multiplier = m_target_multiplier = static_cast< breath_state* >( m_logic.current_state( ) )->get_multiplier( );
	}

	// STRUCTURE DIFF:
	// target: 0x5834d0            base: 0x450650
	// ; void survarium::breath_vibration_calculator::set_breath_holding_params(survarium::breath_holding_params const*) ; target 10 stmts / base 14 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x012 <0x21> | 0x012 <0x1a> | for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )   SIZE
	// --          | <0>         |    EMPTY only base
	// 0x033 <0x17> | 0x02c <0x11> | static_cast< breath_state* >( it )->set_breath_holding_params( params );   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x063 <0x16> | 0x056 <0x15> | m_logic.set_initial_state( m_logic.states( ).front( ) );   SIZE
	// 0x079 <0x3b> | 0x06b <0x2f> | m_current_multiplier = m_target_multiplier = static_cast< breath_state* >( m_logic.current_state( ) )->get_multiplier( );   SIZE
	// .. same ..
	// ; aligned 6, size-diffs 4, quantity-diffs 4
	// VERDICT: STRUCTURE MATCH (shape ok) - for body braced + chained multiplier assignment removed both shape divergences; the 4 SIZE rows are the m_logic.states().front() inline-vs-out-of-line residual (ai::fsm type), NON-steerable  trail: breath_vibration-set_breath_holding_params.md
}

// STATE[100%|DONE]
breath_vibration_calculator::~breath_vibration_calculator( )
{
	while ( ai::fsm_state* state = m_logic.pop_state( ) )
		VOSTOK_DELETE_IMPL( g_allocator, state );
}

// STATE[UNCHECKED]
bool true_predicate( )
{
	return true;

	// FUNCTION BODY
	// <0xcb5c3>|0x003|+0x002:'54'
	// ******
}

// STATE[STUB|INPROGRESS]: NOT matched this unit (large, ~0x3bc bytes). Reconstructed
// shape (verified from 0x5837e0 asm, see _tick.md): new three breath_state subclasses
// (normal/holding/shortbreathing) via VOSTOK_NEW_IMPL(g_allocator, ...), add_state x3,
// then five add_transition( from, to, pred ) calls:
//   (normal,        holding,        boost::bind(&hold_button_state_equals_to,this,true))
//   (holding,       normal,         boost::bind(&hold_button_state_equals_to,this,false))
//   (holding,       shortbreathing, boost::bind(&insufficient_breath,this))
//   (shortbreathing,normal,         true_predicate)
//   (shortbreathing,holding,        true_predicate)
// NEXT STEP to match: switch this cpp's #include from breath_state.h to
// breath_holding_states.{h,inline} (the duplicate breath_state defn conflict must be
// resolved first - the subclass ctors/vtables live there), make initialize_logic
// private in the header (target mangles AAE; currently public -> scores None), write
// the body above, and anchor by calling it transitively (the ctor already calls it).
void breath_vibration_calculator::initialize_logic( )
{
	// LOCALS
	// breath_state* 				holding
	// breath_state* 				normal
	// breath_state* 				shortbreathing
	// ******

	// FUNCTION BODY
	// <0x5937f0>|0x010|+0x086:'59'
	// <0x593876>|0x096|+0x076:'60'
	// <0x5938ec>|0x10c|+0x086:'61'
	// <0>
	// <0x593972>|0x192|+0x00f:'63'
	// <0x593981>|0x1a1|+0x00f:'64'
	// <0x593990>|0x1b0|+0x00f:'65'
	// <0x59399f>|0x1bf|+0x082:'66'
	// <0x593a21>|0x241|+0x082:'67'
	// <0x593aa3>|0x2c3|+0x075:'68'
	// <0x593b18>|0x338|+0x040:'69'
	// <0x593b58>|0x378|+0x040:'70'
	// <0>
	// ******
}

// STATE[94.23%|PARTIAL]: body, control flow, member offsets, virtual dispatch and FPU
// math all match the target structure 1:1 (see _tick.md). The residual is /Od frame-slot
// churn: target frames sub esp,38h vs base 30h, so the saved `this` slot is [ebp-24h]
// (target) vs [ebp-1Ch] (base) and the two m_user loads at the local_time vcall site swap
// registers. Not source-steerable (no missing local/brace/ASSERT - LOCALS dt/current_state/
// current_phase all present, statements identical). math::max/min/sin stay out-of-line as
// in the target.
void breath_vibration_calculator::tick( u32 const current_time_in_ms, float const time_scale )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_user ) );

	if ( current_time_in_ms < m_last_time_in_ms )
		return;

	float const dt = ( current_time_in_ms - m_last_time_in_ms ) * math::epsilon_3 * time_scale;

	m_last_time_in_ms = current_time_in_ms;
	m_logic.tick( );
	breath_state* const current_state = static_cast< breath_state* >( m_logic.current_state( ) );

	current_state->tick( dt );
	m_target_multiplier = current_state->get_multiplier( );

	m_current_multiplier = m_current_multiplier > m_target_multiplier
		? math::max( m_current_multiplier - m_params->multiplier_decrease_speed * dt, m_target_multiplier )
		: math::min( m_current_multiplier + m_params->multiplier_increase_speed * dt, m_target_multiplier );

	float const current_phase = m_user->local_time( current_time_in_ms ) * math::epsilon_3 * math::pi_x2 * time_scale;
	m_horizontal_value	= math::sin( current_phase / m_params->horizontal_peroid ) * m_params->horizontal_amplitude * m_character_multiplier * m_current_multiplier;
	m_vertical_value	= math::sin( current_phase / m_params->vertical_peroid ) * m_params->vertical_amplitude * m_character_multiplier * m_current_multiplier;
	if ( !s_enable_breath_vibration_value )
		m_horizontal_value = m_vertical_value = 0.0f;

	// FUNCTION BODY
	// <0x5935f9>|0x009|+0x00c:'76'	ASSERT( UNKNOWN_EXPRESSION_T( m_user ) )
	// <0x593605>|0x015|+0x00b:'77'	if ( current_time_in_ms < m_last_time_in_ms ) return
	// <0x593610>|0x020|+0x005:'78'	float const dt = ...
	// <0>
	// <0x593615>|0x025|+0x022:'80'	m_last_time_in_ms = current_time_in_ms
	// <0x593637>|0x047|+0x009:'81'	m_logic.tick()
	// <0x593640>|0x050|+0x008:'82'	current_state = static_cast<breath_state*>( m_logic.current_state() )
	// <0>
	// <0x593648>|0x058|+0x015:'84'	current_state->tick( dt )
	// <0x59365d>|0x06d|+0x014:'85'	m_target_multiplier = current_state->get_multiplier()
	// <0x593671>|0x081|+0x01a:'86'
	// <0>
	// <1>
	// <0x59368b>|0x09b|+0x07c:'89'	m_current_multiplier = cond ? max(...) : min(...)
	// <0>
	// <0x593707>|0x117|+0x036:'91'	float const current_phase = ...
	// <0x59373d>|0x14d|+0x039:'92'	m_horizontal_value = ...
	// <0x593776>|0x186|+0x039:'93'	m_vertical_value = ...
	// <0x5937af>|0x1bf|+0x00b:'94'	if ( !s_enable_breath_vibration_value )
	// <0x5937ba>|0x1ca|+0x020:'95'	m_horizontal_value = m_vertical_value = 0.0f
	// <0>
	// ******
}

// STATE[UNCHECKED]
void breath_vibration_calculator::hold_breath( bool value )
{
	if ( value != m_is_breath_holded )
	{
		m_is_breath_holded = value;
	}

	// FUNCTION BODY
	// <0x5934a7>|0x007|+0x00f:'101'
	// <0>
	// <0x5934b6>|0x016|+0x009:'103'
	// <0>
	// ******
}

} // namespace survarium
