////////////////////////////////////////////////////////////////////////////
//	/OPT:REF reachability anchor for the mixing::n_ary_tree_animation_time_calculator carcass.
////////////////////////////////////////////////////////////////////////////

// The n_ary_tree_animation_time_calculator visitor is constructed only by mixer
// (mixer.cpp, not compiled in this build), so /OPT:REF strips the whole class from
// the base EXE and the delinker never produces a body to pair its methods against
// the target. anchor_animation_animation_time_calculator() constructs it and reads
// its public animation_time() accessor. Constructing it pins the visitor vtable, so
// LTCG emits the visit() overrides out-of-line (as the shipped EXE did) for the
// delinker to score.
//
// Self-guarded: the volatile guard is never true at runtime, but the compiler still
// emits every reference, and every argument is sourced through a volatile placeholder
// so LTCG cannot const-fold the carcass bodies away.
//
// fill_time / computed_animation_time are private and have no reachable caller (the
// mixer time-integration path is uncompiled), so the anchor calls fill_time directly
// (void) to pin it; fill_time pins computed_animation_time transitively. Both were
// reconstructed from target asm (the leaf delta-time form + the m_animation
// time_calculator FastDelegate invocation).
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp).
// TEMPORARY - retire once the real mixer call graph reaches the calculator.

#include "pch.h"

#include "mixing_n_ary_tree_animation_time_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"

namespace vostok {

void anchor_animation_animation_time_calculator( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	using namespace animation::mixing;

	static n_ary_tree_animation_node* volatile	s_animation		= 0;
	static u32 volatile							s_start_time	= 0;
	static float volatile						s_start_anim	= 0;
	static u32 volatile							s_target_time	= 0;
	static bool volatile						s_read_only		= false;

	n_ary_tree_animation_time_calculator	calculator(
		*s_animation,
		s_start_time,
		s_start_anim,
		s_target_time,
		s_read_only
	);

	static float volatile	s_sink_float	= 0;
	s_sink_float	= calculator.animation_time( );

	// fill_time (and the computed_animation_time it calls) are private and otherwise
	// uncalled; the friend grant lets the anchor pin both.
	static float volatile	s_scale			= 0;
	static float volatile	s_anim_before	= 0;
	static u32 volatile		s_scale_start	= 0;
	calculator.fill_time( s_scale, s_anim_before, s_scale_start );
}

} // namespace vostok
