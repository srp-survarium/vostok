////////////////////////////////////////////////////////////////////////////
//	/OPT:REF reachability anchor for the mixing::n_ary_tree_weaver carcass.
////////////////////////////////////////////////////////////////////////////

// The n_ary_tree_weaver visitor class is constructed only by mixer::set_target
// (mixer.cpp, not compiled in this build), so /OPT:REF strips the whole class from the
// base EXE and the delinker never produces a body to pair its ~15 methods against the
// target. anchor_animation_weaver() constructs the weaver and calls its public surface
// (ctor -> animations_root/interpolators_root/interpolators_count). Constructing the
// weaver pins its visitor vtable, so LTCG emits the five private virtual visit() overrides
// (and the helpers they call: add_interpolator, join_animations, clean<T>, update_weights,
// is_unique_animation_lexeme) out-of-line for the delinker to score, as the shipped EXE did.
//
// Self-guarded: the volatile guard is never true at runtime, but the compiler still emits
// every reference, and every argument is sourced through a volatile placeholder so LTCG
// cannot const-fold the carcass bodies away.
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp).
// Retire once the real mixer::set_target call graph reaches the weaver.

#include "pch.h"

#include <vostok/memory_buffer.h>
#include "mixing_n_ary_tree_weaver.h"
#include <vostok/animation/mixing_binary_tree_animation_node.h>

namespace vostok {

void anchor_animation_weaver( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	using namespace animation::mixing;

	static vostok::mutable_buffer* volatile	s_buffer	= 0;
	vostok::mutable_buffer&					buffer		= *s_buffer;

	n_ary_tree_weaver	weaver( buffer );

	static binary_tree_animation_node* volatile	s_sink_node	= 0;
	static volatile u32							s_sink_u32	= 0;
	s_sink_node	= weaver.animations_root( );
	s_sink_u32	= reinterpret_cast< u32 >( weaver.interpolators_root( ) );
	s_sink_u32	= weaver.interpolators_count( );
}

} // namespace vostok
