// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/ai/fsm.h>
#include "ai_memory.h"

namespace vostok {
namespace ai {

// claude@NOTE: STRUCTURE MATCH (init-list ctor). Capped by the global de-inline
// knob - the target out-of-lines intrusive_list<fsm_state,...>::intrusive_list()
// while our base inlines it (declared `inline` in intrusive_list_inline.h); flipping
// that keyword is an off-limits cross-unit change (pattern template-accessor-deinline-global-knob).
fsm::fsm( ) :
	m_current_state( 0 )
{
}

// claude@NOTE: STRUCTURE MATCH; sole residual is push_back inline-vs-call (same
// global de-inline knob as the ctor) plus the this-in-EAX LTCG convention.
void fsm::add_state( fsm_state* state )
{
	ASSERT( UNKNOWN_EXPRESSION_T( state ) );
	m_states.push_back( state );
}

void fsm::add_transition( fsm_state* from, fsm_state* to, boost::function<bool()> const& transition_predicate )
{
	fsm_state_transition* const transition	= NEW( fsm_state_transition );
	transition->predicate					= transition_predicate;
	transition->target_state				= to;
	from->transitions.push_back				( transition );
}

fsm_state_list& fsm::states( )
{
	return m_states;
}

void fsm::set_initial_state( fsm_state* initial_state )
{
	if ( m_current_state == initial_state )
		return;

	if ( m_current_state )
		m_current_state->finalize( );

	m_current_state = initial_state;

	if ( m_current_state )
		m_current_state->initialize( );
}

void fsm::tick( )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( m_current_state->is_ready_for_transition( ) )
	{
		for ( fsm_state_transition* i = m_current_state->transitions.front( ); i; i = i->next )
		{
			if ( i->predicate( ) )
			{
				m_current_state->finalize( );
				m_current_state = i->target_state;
				m_current_state->initialize( );
				break;
			}
		}
	}

	m_current_state->execute( );
}

void fsm::clear_transitions( )
{
	for ( fsm_state* j = m_states.front( ); j; j = m_states.get_next_of_object( j ) )
		while ( fsm_state_transition* i = j->transitions.pop_front( ) )
			DELETE( i );
}

fsm_state* fsm::pop_state( )
{
	return m_states.pop_front( );
}

} // namespace ai
} // namespace vostok
