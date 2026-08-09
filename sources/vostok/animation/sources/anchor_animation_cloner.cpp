////////////////////////////////////////////////////////////////////////////
//	Reachability anchor for mixing::n_ary_tree_cloner's optimized call graph.
////////////////////////////////////////////////////////////////////////////

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

	static n_ary_tree_animation_node* volatile	s_anim	= 0;

	static n_ary_tree_animation_node* volatile	s_sink_anim	= 0;
	s_sink_anim	= constructor.add_animation( *s_anim, s_anim );
}

} // namespace vostok
