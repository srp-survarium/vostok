////////////////////////////////////////////////////////////////////////////
//	/OPT:REF reachability anchor for the mixing::n_ary_tree_comparer carcass.
////////////////////////////////////////////////////////////////////////////

// The n_ary_tree_comparer visitor class is constructed only by mixer::set_target_impl
// (mixer.cpp, not compiled in this build), so /OPT:REF strips the whole class from the
// base EXE and the delinker never produces a body to pair its ~28 methods against the
// target. anchor_animation_comparer() constructs the comparer and calls its public
// surface (ctor -> process_interpolators/merge_trees, equal, needed_buffer_size) plus the
// comparison predicate, so LTCG emits each bodied member out-of-line (as the shipped EXE
// did) for the delinker to score. Members still carrying empty STUB bodies inline away and
// stay TARGET_ONLY until their bodies are reconstructed - the anchor reaches them, but a
// trivial body is not emitted standalone.
//
// Self-guarded: the volatile guard is never true at runtime, but the compiler still emits
// every reference, and every argument is sourced through a volatile placeholder so LTCG
// cannot const-fold the carcass bodies away.
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp).
// Retire once the real mixer::set_target call graph reaches the comparer.

#include "pch.h"

#include <vostok/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_comparer.h"
#include "mixing_n_ary_tree_animation_node.h"

namespace vostok {

void anchor_animation_comparer( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	using namespace animation::mixing;

	static animation::mixing::n_ary_tree const* volatile	s_from	= 0;
	static animation::mixing::n_ary_tree const* volatile	s_to	= 0;
	static u32 volatile										s_time	= 0;

	animation::mixing::n_ary_tree const&	from	= *s_from;
	animation::mixing::n_ary_tree const&	to		= *s_to;

	n_ary_tree_comparer	comparer( from, to, s_time );

	static volatile bool s_sink_bool = false;
	static volatile u32  s_sink_u32  = 0;
	s_sink_bool	= comparer.equal( );
	s_sink_u32	= comparer.needed_buffer_size( );
	s_sink_u32	= comparer.animations_count( );
	s_sink_u32	= comparer.animated_objects_count( );

	// the comparison predicate has no reachable caller either; construct and call it.
	static animation::mixing::n_ary_tree_animation_node const* volatile	s_node	= 0;
	animation::mixing::n_ary_tree_animation_node const&	node	= *s_node;

	animation_comparer_predicate	predicate( s_sink_bool, s_sink_bool );
	s_sink_u32	= predicate( node, node );
}

} // namespace vostok
