////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/breath_vibration_calculator.h>
#include <vostok/game_core/breath_state.h>
#include <vostok/game_core/breath_holding_params.h>

namespace survarium {
/*
// STATE[STUB]
// void survarium::`dynamic initializer for 's_enable_breath_vibration_cc''()
void `dynamic initializer for 's_enable_breath_vibration_cc''( )
{
	// FUNCTION BODY
	// <0x7db2f0>|0x000|      :'15'	{
	// ******
}
*/
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

// STATE[76.80%|INPROGRESS]: structure-verified MISMATCH (both quantity+size). The
// 76.80% hides TWO source-shape divergences (see embedded structure-diff below):
//   (A) for-loop body is BRACED in target - its back-edge `jmp` lands on its own
//       source line L37 (`0x4a <0x2>`, ONLY target). Our brace-less single-statement
//       for folds that jmp into L36, so we are missing L37. Brace the for body:
//           for ( ... ) { static_cast<...>( it )->set_breath_holding_params( params ); }
//   (B) the two multiplier writes are a CHAINED assignment in target (one statement
//       L41, 0x3b bytes); we split them into L41 + a separate L42
//       `m_current_multiplier = m_target_multiplier;` (ONLY base). Chain them:
//           m_current_multiplier = m_target_multiplier = static_cast<...>( ... )->get_multiplier( );
// The `if ( m_params )` braces are NOT the problem - both sides codegen the if
// brace-less at function tail (no scope-close marker on either side); the earlier
// "early-return guard" hypothesis was wrong. (Residual: fsm::states()/front() inlined
// in base vs out-of-line in target - blocked on the ai fsm type.) A genuine restructure
// (apply A+B) plus rebuild is a matcher's job; not done here.
void breath_vibration_calculator::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;

	for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )
		static_cast< breath_state* >( it )->set_breath_holding_params( params );

	if ( m_params )
	{
		m_breath_holding_reserve	= m_params->max_breath_holding_time;
		m_logic.set_initial_state( m_logic.states( ).front( ) );
		m_target_multiplier			= static_cast< breath_state* >( m_logic.current_state( ) )->get_multiplier( );
		m_current_multiplier		= m_target_multiplier;
	}

	// STRUCTURE DIFF:
	// target: 0x5834d0            base: 0x4574a0
	// ; void survarium::breath_vibration_calculator::set_breath_holding_params(survarium::breath_holding_params const*) ; target 8 stmts / base 11 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x012 <0x21> | 0x012 <0x1a> | for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )   SIZE
	// 0x033 <0x17> | 0x02c <0x13> | static_cast< breath_state* >( it )->set_breath_holding_params( params );   SIZE
	// --          | <0>         |    EMPTY only base
	// 0x04a <0x2> | --          | L37   ONLY target
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x063 <0x16> | 0x056 <0x15> | m_logic.set_initial_state( m_logic.states( ).front( ) );   SIZE
	// 0x079 <0x3b> | 0x06b <0x23> | m_target_multiplier			= static_cast< breath_state* >( m_logic.current_state( ) )->get_multiplier( );   SIZE
	// --          | 0x08e <0xc> | m_current_multiplier		= m_target_multiplier;   ONLY base
	// .. same ..
	// ; aligned 3, size-diffs 4, quantity-diffs 5
	// VERDICT: STRUCTURE MISMATCH (both) - brace the for body (target back-edge on own line L37) and chain m_current=m_target=...->get_multiplier() into one stmt  trail: breath_vibration-set_breath_holding_params.md
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

// STATE[STUB]
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

// STATE[STUB]
void breath_vibration_calculator::tick( u32 const current_time_in_ms, float const time_scale )
{
	// LOCALS
	// float 						dt
	// breath_state* 				current_state
	// float 						current_phase
	// ******

	// CALL SITE INFO
	// <0x59366f> -> void <unknown>(const float)
	// <0x59371c> -> u32 <unknown>(const u32) const
	// ******

	// FUNCTION BODY
	// <0x5935f9>|0x009|+0x00c:'76'
	// <0x593605>|0x015|+0x00b:'77'
	// <0x593610>|0x020|+0x005:'78'
	// <0>
	// <0x593615>|0x025|+0x022:'80'
	// <0x593637>|0x047|+0x009:'81'
	// <0x593640>|0x050|+0x008:'82'
	// <0>
	// <0x593648>|0x058|+0x015:'84'
	// <0x59365d>|0x06d|+0x014:'85'
	// <0x593671>|0x081|+0x01a:'86'
	// <0>
	// <1>
	// <0x59368b>|0x09b|+0x07c:'89'
	// <0>
	// <0x593707>|0x117|+0x036:'91'
	// <0x59373d>|0x14d|+0x039:'92'
	// <0x593776>|0x186|+0x039:'93'
	// <0x5937af>|0x1bf|+0x00b:'94'
	// <0x5937ba>|0x1ca|+0x020:'95'
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
