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

static bool s_enable_breath_vibration_value = true;
static console_commands::cc_bool s_enable_breath_vibration_cc( "breath_vibration_enable", s_enable_breath_vibration_value, true, console_commands::command_type_user_specific );
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
}

breath_vibration_calculator::~breath_vibration_calculator( )
{
	while ( ai::fsm_state* state = m_logic.pop_state( ) )
		VOSTOK_DELETE_IMPL( g_allocator, state );
}

bool true_predicate( )
{
	return true;
}

// STATE[STUB]: NOT matched this unit (large ~0x3bc; fsm-build with 3 breath_state
// subclasses + 5 boost::bind/true_predicate add_transitions). Reconstructed body + concrete
// next steps (header move to private AAE, breath_holding_states includes) in _tick.md.
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
}

void breath_vibration_calculator::hold_breath( bool value )
{
	if ( value != m_is_breath_holded )
	{
		m_is_breath_holded = value;
	}
}

} // namespace survarium
