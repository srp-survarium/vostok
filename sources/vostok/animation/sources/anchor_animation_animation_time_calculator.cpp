////////////////////////////////////////////////////////////////////////////
//	Reachability anchor for the animation-time calculator's optimized call graph.
////////////////////////////////////////////////////////////////////////////

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
