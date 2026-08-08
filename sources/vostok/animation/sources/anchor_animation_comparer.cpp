////////////////////////////////////////////////////////////////////////////
//	Reachability anchor for mixing::n_ary_tree_comparer's optimized call graph.
////////////////////////////////////////////////////////////////////////////

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

	static animation::mixing::n_ary_tree_animation_node const* volatile	s_node	= 0;
	animation::mixing::n_ary_tree_animation_node const&	node	= *s_node;

	animation_comparer_predicate	predicate( s_sink_bool, s_sink_bool );
	s_sink_u32	= predicate( node, node );
}

} // namespace vostok
