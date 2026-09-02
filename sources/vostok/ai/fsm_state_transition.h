// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef AI_FSM_STATE_TRANSITION_H_INCLUDED
#define AI_FSM_STATE_TRANSITION_H_INCLUDED

namespace vostok {
namespace ai {

struct fsm_state;

struct fsm_state_transition {
public:
	inline	explicit	fsm_state_transition	( ) { }
	inline				~fsm_state_transition	( ) { }

public:
	/* 0x0000 */	boost::function< bool() >	predicate;
	/* 0x0020 */	fsm_state*					target_state;
	/* 0x0024 */	fsm_state_transition*		next;
}; // struct fsm_state_transition

STATIC_SIZE_ASSERT(fsm_state_transition, 0x28);


typedef intrusive_list<
	fsm_state_transition,
	fsm_state_transition *,
	&fsm_state_transition::next,
	threading::single_threading_policy,
	size_policy,
	no_debug_policy > fsm_state_transition_list;


} // namespace ai
} // namespace vostok

#endif // #ifndef AI_FSM_STATE_TRANSITION_H_INCLUDED
