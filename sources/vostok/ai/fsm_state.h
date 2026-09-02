// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AI_FSM_STATE_H_INCLUDED
#define AI_FSM_STATE_H_INCLUDED

#include <vostok/ai/fsm_state_transition.h>

namespace vostok {
namespace ai {

struct VOSTOK_NOVTABLE fsm_state {
	virtual			~fsm_state					( ) = 0;

	virtual	void	initialize					( ) = 0;
	virtual	void	execute						( ) = 0;
	virtual	void	finalize					( ) = 0;
	virtual	bool	is_ready_for_transition		( ) const = 0;

public:
	/* 0x0004 */	fsm_state*					next;
	/* 0x0008 */	fsm_state_transition_list	transitions;
}; // struct fsm_state

inline fsm_state::~fsm_state( )
{
}

STATIC_SIZE_ASSERT(fsm_state, 0x18);

typedef intrusive_list<
	fsm_state,
	fsm_state *,
	&fsm_state::next,
	threading::single_threading_policy,
	size_policy,
	no_debug_policy > fsm_state_list;


} // namespace ai
} // namespace vostok

#endif // #ifndef AI_FSM_STATE_H_INCLUDED
