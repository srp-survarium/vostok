// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef AI_FSM_H_INCLUDED
#define AI_FSM_H_INCLUDED

#include <vostok/ai/fsm_state.h>

namespace vostok {
namespace ai {

class fsm {
public:
	explicit						fsm					( );

			void					add_state			( fsm_state* state );
	inline	void					remove_state		( fsm_state* arg_0 ) { /* no source */ }

			void					add_transition		( fsm_state* from, fsm_state* to, boost::function<bool()> const& transition_predicate );
	inline	void					remove_transition	( fsm_state* arg_0, fsm_state const* arg_1 ) { /* no source */ }

			void					set_initial_state	( fsm_state* initial_state );
	inline	fsm_state*				current_state		( ) const { return m_current_state; }

			void					tick				( );

			void					clear_transitions	( );

			fsm_state*				pop_state			( );

	// claude@MATCH: the non-const overload is OUT-OF-LINE in the target
	// (`intrusive_list<...>& fsm::states()` is a real standalone target symbol,
	// called with the LTCG this-in-EAX convention from jump_logic & friends);
	// the const overload stays inline (const call sites, e.g.
	// weapon_user_animations_selector::serialize, show no states() call).
	// Declared before the const overload to match the target record order.
			fsm_state_list&			states				( );
	inline	fsm_state_list const&	states				( ) const { return m_states; }


private:
	/* 0x0000 */	fsm_state_list		m_states;
	/* 0x0010 */	fsm_state*			m_current_state;
}; // class fsm

STATIC_SIZE_ASSERT(fsm, 0x14);

} // namespace ai
} // namespace vostok

#endif // #ifndef AI_FSM_H_INCLUDED
