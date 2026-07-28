////////////////////////////////////////////////////////////////////////////
//	/OPT:REF reachability anchor for the mixing::n_ary_tree_time_in_ms_calculator carcass.
////////////////////////////////////////////////////////////////////////////

// The n_ary_tree_time_in_ms_calculator visitor is constructed only by mixer
// (mixer.cpp, not compiled in this build), so /OPT:REF strips the whole class from
// the base EXE and the delinker never produces a body to pair its methods against
// the target. anchor_animation_time_in_ms_calculator() constructs it and reads its
// public accessors. Constructing it pins the visitor vtable, so LTCG emits the
// visit() overrides out-of-line (as the shipped EXE did) for the delinker to score.
//
// Self-guarded: the volatile guard is never true at runtime, but the compiler still
// emits every reference, and every argument is sourced through a volatile placeholder
// so LTCG cannot const-fold the carcass bodies away.
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp).
// TEMPORARY - retire once the real mixer call graph reaches the calculator.

#include "pch.h"

#include "mixing_n_ary_tree_time_in_ms_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"

namespace vostok {

void anchor_animation_time_in_ms_calculator( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	using namespace animation::mixing;

	static n_ary_tree_animation_node* volatile	s_animation		= 0;
	static u32 volatile							s_start_time	= 0;
	static float volatile						s_start_anim	= 0;
	static float volatile						s_target_anim	= 0;
	static u16 volatile							s_event_type	= 0;

	n_ary_tree_time_in_ms_calculator	calculator(
		*s_animation,
		s_start_time,
		s_start_anim,
		s_target_anim,
		s_event_type
	);

	static u32 volatile		s_sink_u32		= 0;
	static u16 volatile		s_sink_u16		= 0;
	static float volatile	s_sink_float	= 0;
	s_sink_u32		= calculator.time_in_ms( );
	s_sink_u16		= calculator.event_type( );
	s_sink_float	= calculator.event_time( );
}

} // namespace vostok
