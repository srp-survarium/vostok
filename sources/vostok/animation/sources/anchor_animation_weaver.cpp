////////////////////////////////////////////////////////////////////////////
//	Reachability anchor for mixing::n_ary_tree_weaver's optimized call graph.
////////////////////////////////////////////////////////////////////////////

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
