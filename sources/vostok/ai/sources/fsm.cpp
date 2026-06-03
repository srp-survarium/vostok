////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/ai/fsm.h>

namespace vostok {
namespace ai {

// STATE[STUB]
// vostok::ai::fsm::fsm()
fsm::fsm( )
{
	// FUNCTION BODY[0x579460]: 0
	// ******
}

// STATE[STUB]
// void vostok::ai::fsm::add_state(vostok::ai::fsm_state*)
void fsm::add_state( fsm_state* state )
{
	// FUNCTION BODY[0x579430]: 2
	// <0x57943a>|0x00a|+0x00c:'23'
	// <0x579446>|0x016|+0x00d:'24'
	// ******
}

// STATE[STUB]
// void vostok::ai::fsm::add_transition(vostok::ai::fsm_state*, vostok::ai::fsm_state*, boost::function<bool __cdecl(void)> const&)
void fsm::add_transition( fsm_state* from, fsm_state* to, boost::function<bool()> const& transition_predicate )
{
	// LOCALS
	// fsm_state_transition* 		transition
	// ******

	// FUNCTION BODY[0x5795c0]: 4
	// <0x5795ca>|0x00a|+0x04e:'35'
	// <0x579618>|0x058|+0x028:'36'
	// <0x579640>|0x080|+0x009:'37'
	// <0x579649>|0x089|+0x011:'38'
	// ******
}

// STATE[STUB]
// void vostok::ai::fsm::set_initial_state(vostok::ai::fsm_state*)
void fsm::set_initial_state( fsm_state* initial_state )
{
	// CALL SITE INFO
	// <0x5793e0> -> void <unknown>()
	// <0x579405> -> void <unknown>()
	// ******

	// FUNCTION BODY[0x5793b0]: 12
	// <0x5793b7>|0x007|+0x00b:'63'
	// <0x5793c2>|0x012|+0x002:'64'
	// <0>
	// <0x5793c4>|0x014|+0x009:'66'
	// <0>
	// <1>
	// <0x5793cd>|0x01d|+0x015:'69'
	// <0>
	// <1>
	// <0x5793e2>|0x032|+0x009:'72'
	// <0x5793eb>|0x03b|+0x009:'73'
	// <0x5793f4>|0x044|+0x013:'74'
	// ******
}

// STATE[STUB]
// void vostok::ai::fsm::tick()
void fsm::tick( )
{
	// LOCALS
	// fsm_state_transition* 		i<1>
	// ******

	// CALL SITE INFO
	// <0x579522> -> bool <unknown>() const
	// <0x579571> -> void <unknown>()
	// <0x579599> -> void <unknown>()
	// <0x5795b6> -> void <unknown>()
	// ******

	// FUNCTION BODY[0x5794f0]: 17
	// <0x5794ff>|0x00f|+0x00c:'79'
	// <0>
	// <0x57950b>|0x01b|+0x020:'81'
	// <0>
	// <0x57952b>|0x03b|+0x020|[1]:'83'
	// <0>
	// <0x57954b>|0x05b|+0x00f:'85'
	// <0>
	// <0x57955a>|0x06a|+0x019:'87'
	// <0x579573>|0x083|+0x00f:'88'
	// <0x579582>|0x092|+0x019:'89'
	// <0x57959b>|0x0ab|+0x002:'90'
	// <0>
	// <0x57959d>|0x0ad|+0x002:'92'
	// <0>
	// <1>
	// <0x57959f>|0x0af|+0x019:'95'
	// ******
}

// STATE[STUB]
// void vostok::ai::fsm::clear_transitions()
void fsm::clear_transitions( )
{
	// LOCALS
	// fsm_state* 					j<1>
	// fsm_state_transition* 		i<2>
	// ******

	// FUNCTION BODY[0x579480]: 3
	// <0x579489>|0x009|+0x01e|[1]:'100'
	// <0x5794a7>|0x027|+0x014|[2]:'101'
	// <0x5794bb>|0x03b|+0x02a:'102'
	// ******
}

// STATE[STUB]
// vostok::ai::fsm_state* vostok::ai::fsm::pop_state()
fsm_state* fsm::pop_state( )
{
	return NULL;

	// FUNCTION BODY[0x579410]: 1
	// <0x579419>|0x009|+0x008:'107'
	// ******
}

} // namespace ai
} // namespace vostok
