// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/breath_vibration_calculator.h>
#include <vostok/game_core/breath_holding_params.h>
#include "breath_holding_states.h"
#include "breath_holding_states_inline.h"
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

// claude@NOTE: residual = the two m_logic.states( ).front( ) sites. Target calls
// intrusive_list::front with `this` in EAX (no `mov ecx,eax`); base inlines/calls it
// with `this` in ECX, leaving a 2-byte `mov ecx,eax` per site. That is the global
// intrusive_list front()-de-inline knob (intrusive_list_inline.h) - off-limits from
// this TU. See patterns/template-accessor-deinline-global-knob.md.
void breath_vibration_calculator::set_breath_holding_params( breath_holding_params const* params )
{
	m_params = params;

	for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )
	{
		static_cast_checked< breath_state* >( it )->set_breath_holding_params( params );
	}

	if ( m_params )
	{
		m_breath_holding_reserve	= m_params->max_breath_holding_time;
		m_logic.set_initial_state( m_logic.states( ).front( ) );
		m_current_multiplier = m_target_multiplier = static_cast_checked< breath_state* >( m_logic.current_state( ) )->get_multiplier( );
	}
}

breath_vibration_calculator::~breath_vibration_calculator( )
{
	while ( ai::fsm_state* state = m_logic.pop_state( ) )
	{
		VOSTOK_DELETE_IMPL( g_allocator, state );
	}
}

bool true_predicate( )
{
	return true;
}

// claude@NOTE: STRUCTURE MATCH (11/11). Residual is a 4-byte frame delta - the target
// saves ESI (push esi / sub esp,128h) and uses it as `this` for the trailing
// boost::function::clear() calls, shifting every [ebp-XX] slot by 8; base keeps ECX
// (no esi, sub esp,130h). Whole-program LTCG register-budget choice, not source-steerable.
void breath_vibration_calculator::initialize_logic( )
{
	breath_state* normal			= VOSTOK_NEW_IMPL( g_allocator, breath_state_normal )( m_breath_holding_reserve );
	breath_state* holding			= VOSTOK_NEW_IMPL( g_allocator, breath_state_holding )( m_breath_holding_reserve );
	breath_state* shortbreathing	= VOSTOK_NEW_IMPL( g_allocator, breath_state_shortbreathing )( m_breath_holding_reserve );

	m_logic.add_state( normal );
	m_logic.add_state( holding );
	m_logic.add_state( shortbreathing );

	m_logic.add_transition( normal, holding, boost::bind( &breath_vibration_calculator::hold_button_state_equals_to, this, true ) );
	m_logic.add_transition( holding, normal, boost::bind( &breath_vibration_calculator::hold_button_state_equals_to, this, false ) );
	m_logic.add_transition( holding, shortbreathing, boost::bind( &breath_vibration_calculator::insufficient_breath, this ) );
	m_logic.add_transition( shortbreathing, normal, &true_predicate );
	m_logic.add_transition( shortbreathing, holding, &true_predicate );
}

void breath_vibration_calculator::tick( u32 const current_time_in_ms, float const time_scale )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_user ) );

	if ( current_time_in_ms < m_last_time_in_ms )
		return;

	float const dt = ( current_time_in_ms - m_last_time_in_ms ) * math::epsilon_3 * time_scale;

	m_last_time_in_ms = current_time_in_ms;
	m_logic.tick( );
	breath_state* const current_state = static_cast_checked< breath_state* >( m_logic.current_state( ) );

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
