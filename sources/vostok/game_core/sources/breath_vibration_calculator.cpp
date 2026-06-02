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

// STATE[76.80%|INPROGRESS]: NOT verified exact - the bracing is unconfirmed. The
// FUNCTION BODY structure (below) shows '38'-'41' as four plain statements at function
// scope with NO block-open marker, yet this source wraps them in `if ( m_params ) { ... }`
// - that `{` would appear as a `<n>` (no-address) line in the structure, which is ABSENT.
// Likely an early `return` guard (`if ( !m_params ) return;`, no braces) instead. Needs
// independent verification + re-diff on a faster machine; do NOT trust the 76.80% as
// "matched body". (Separately, fsm::states()/front() are inlined in base vs out-of-line
// in target - that residual is blocked on the ai fsm type.) See ...accessors.md.
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

	// FUNCTION BODY
	// <0x5934d9>|0x009|+0x009:'34'
	// <0x5934e2>|0x012|+0x021|[1]:'35'
	// <0x593503>|0x033|+0x017:'36'
	// <0x59351a>|0x04a|+0x002:'37'
	// <0x59351c>|0x04c|+0x009:'38'
	// <0x593525>|0x055|+0x00e:'39'
	// <0x593533>|0x063|+0x016:'40'
	// <0x593549>|0x079|+0x03b:'41'
	// <0>
	// ******
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
