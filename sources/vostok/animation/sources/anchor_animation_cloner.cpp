////////////////////////////////////////////////////////////////////////////
//	/OPT:REF reachability anchor for the mixing::n_ary_tree_cloner carcass.
////////////////////////////////////////////////////////////////////////////

// The n_ary_tree_cloner visitor is constructed only by mixer::set_target_impl
// (mixer.cpp, not compiled in this build), so /OPT:REF strips the whole class from
// the base EXE and the delinker never produces a body to pair its methods against
// the target. anchor_animation_cloner() constructs the cloner and calls its public
// surface (ctor + initialize + the three public clone() overloads + interpolators
// accessors). Constructing it pins the visitor vtable, so LTCG emits the visit()
// overrides and the propagate<>/new_constructed<>/private clone() helpers
// out-of-line (as the shipped EXE did) for the delinker to score.
//
// Self-guarded: the volatile guard is never true at runtime, but the compiler still
// emits every reference, and every argument is sourced through a volatile placeholder
// so LTCG cannot const-fold the carcass bodies away.
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp).
// TEMPORARY - retire once the real mixer::set_target call graph reaches the cloner.

#include "pch.h"

#include <vostok/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_cloner.h"
#include "mixing_n_ary_tree_base_node.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"

namespace vostok {

void anchor_animation_cloner( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	using namespace animation::mixing;

	static n_ary_tree_transition_tree_constructor* volatile	s_constructor	= 0;
	static u32 volatile										s_start_time	= 0;

	n_ary_tree_transition_tree_constructor&	constructor	= *s_constructor;

	n_ary_tree_cloner	cloner( constructor, s_start_time );

	static n_ary_tree const* volatile	s_from	= 0;
	static n_ary_tree const* volatile	s_to	= 0;
	cloner.initialize( *s_from, *s_to );

	static n_ary_tree_base_node* volatile	s_node	= 0;
	static float volatile					s_scale	= 0;
	static float volatile					s_interval = 0;

	static n_ary_tree_base_node* volatile	s_sink_node	= 0;
	s_sink_node	= cloner.clone( *s_node );
	s_sink_node	= cloner.clone( *s_node, s_scale, s_interval );
	s_sink_node	= cloner.clone( *s_node, s_scale );

	static animation::base_interpolator const** volatile	s_sink_interps	= 0;
	static u32 volatile										s_sink_count	= 0;
	s_sink_interps	= cloner.interpolators( );
	s_sink_count	= cloner.interpolators_count( );

	// Pin the real public root change_animation(); LTCG then emits the genuine
	// constructor call graph (new_animation -> add_operands -> the new_*_transition
	// node-construction leaves) out-of-line, as the shipped EXE did. The leaves are
	// reached through their TRUE callers here, not enumerated in this anchor.
	static n_ary_tree_animation_node* volatile	s_anim	= 0;
	static bool volatile						s_flag	= false;
	constructor.change_animation( *s_anim, *s_anim, s_anim, s_flag );

	// Pin the other public root add_animation() (the single-animation builder, reached in
	// the real call graph through add_synchronization_group/merge_trees, both still STUBs).
	// Un-DCEs add_animation + its leaf new_weight_transition(base_interpolator const&,float,float).
	// TEMPORARY - retire once merge_trees reaches add_animation.
	static n_ary_tree_animation_node* volatile	s_sink_anim	= 0;
	s_sink_anim	= constructor.add_animation( *s_anim, s_anim );
}

} // namespace vostok
