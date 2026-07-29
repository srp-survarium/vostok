////////////////////////////////////////////////////////////////////////////
//	/OPT:REF anchor for time-calculation visitors not reached by the game graph.
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "mixing_n_ary_tree_time_in_ms_calculator.h"
#include "mixing_n_ary_tree_target_time_scale_calculator.h"
#include "mixing_n_ary_tree_time_scale_start_time_modifier.h"
#include "mixing_n_ary_tree_animation_node.h"
#include <vostok/animation/bone_animation.h>
#include <vostok/animation/bone_names.h>
#include <vostok/animation/cubic_spline_skeleton_animation.h>

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

	n_ary_tree_target_time_scale_calculator target_scale( *s_animation );
	s_sink_float	= target_scale.result( );

	n_ary_tree_time_scale_start_time_modifier(
		*s_animation,
		s_start_time,
		s_start_anim
	);

	static pcvoid volatile s_sink_pointer = 0;
	void ( animation::bone_names::* const create_bone_names )(
		configs::binary_config_ptr const&,
		void*
	) = &animation::bone_names::create_internals_in_place;
	s_sink_pointer = *(pcvoid const*)&create_bone_names;

	u32 (* const count_bone_animation)(
		animation::bi_spline_bone_animation_baked const&
	) = &animation::bone_animation::count_internal_memory_size;
	s_sink_pointer = *(pcvoid const*)&count_bone_animation;

	void ( animation::bone_animation::* const create_bone_animation )(
		animation::bi_spline_bone_animation_baked const&,
		void*
	) = &animation::bone_animation::create_internals_in_place;
	s_sink_pointer = *(pcvoid const*)&create_bone_animation;

	u32 (* const count_skeleton_animation)(
		animation::bi_spline_skeleton_animation_baked const&
	) = &animation::cubic_spline_skeleton_animation::count_memory_size;
	s_sink_pointer = *(pcvoid const*)&count_skeleton_animation;

	animation::cubic_spline_skeleton_animation* (* const create_skeleton_animation)(
		pvoid,
		animation::bi_spline_skeleton_animation_baked const&
	) = &animation::cubic_spline_skeleton_animation::new_animation;
	s_sink_pointer = *(pcvoid const*)&create_skeleton_animation;
}

} // namespace vostok
