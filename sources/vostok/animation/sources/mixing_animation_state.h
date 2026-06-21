////////////////////////////////////////////////////////////////////////////
//	Created		: 21.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_ANIMATION_STATE_H_INCLUDED
#define MIXING_ANIMATION_STATE_H_INCLUDED

#include <vostok/animation/cubic_spline_skeleton_animation.h>
#include "mixing_n_ary_tree_event_iterator.h"

namespace vostok {
namespace animation {

struct bone_mixer_data;

namespace mixing {

class animation_interval;
class animation_clip;
class n_ary_tree_animation_node;

struct VOSTOK_ANIMATION_API object_movement {
	math::quaternion			rotation;
	float3						translation;
	float3						scale;
}; // struct object_movement

STATIC_SIZE_ASSERT(object_movement, 0x28);

struct VOSTOK_ANIMATION_API bone_matrices_computer_data {
	inline	bone_matrices_computer_data	( ) : pinned_animation( 0 ) { }

	object_movement				previous_object_movement;
	object_movement				accumulated_object_movement;

	resources::pinned_ptr_const< cubic_spline_skeleton_animation >	pinned_animation;
}; // struct bone_matrices_computer_data

STATIC_SIZE_ASSERT(bone_matrices_computer_data, 0x5C);

#if 0
struct time_calculator_data {
	u32							last_integration_interval_id;
	float						last_integration_interval_value;
}; // struct time_calculator_data
#endif

struct VOSTOK_ANIMATION_API animation_state {
								animation_state	(
									n_ary_tree_animation_node&		animation_node,
									u32								time_in_ms,
									u16								initial_event_types,
									u32								animation_interval_id,
									u32								previous_animation_interval_id,
									float							animation_interval_time,
									float							animation_time_threshold,
									float							weight,
									subscribed_channel*&			channels_head,
									bool							is_freezed
								);
	inline animation_state&		get_this		( ) { return *this; }

public:
	/* 0x0000 */	bone_matrices_computer_data		bone_matrices_computer;
	/* 0x005c */	u32								animation_interval_id;
	/* 0x0060 */	u32								previous_animation_interval_id;
	/* 0x0064 */	float							animation_interval_time;
	/* 0x0068 */	float							weight;
	/* 0x006c */	float							animation_time;
	/* 0x0070 */	float							animation_time_threshold;
	/* 0x0074 */	bool							are_there_any_weight_transitions;
	/* 0x0075 */	bool							is_freezed;
	/* 0x0078 */	n_ary_tree_event_iterator		event_iterator;

}; // struct animation_state

STATIC_SIZE_ASSERT(animation_state, 0xB4);

struct animation_state_params {
	inline		animation_state_params	(
					const u16						initial_event_types,
					const u32						animation_interval_id,
					const float						animation_interval_time,
					const float						animation_time_threshold,
					const float						weight,
					animation_state const* const	previous
				) :
					animation_interval_id	( animation_interval_id ),
					animation_interval_time	( animation_interval_time ),
					animation_time_threshold( animation_time_threshold ),
					weight					( weight ),
					initial_event_types		( initial_event_types ),
					previous				( previous ) { }

public:
	/* 0x0000 */	u32							animation_interval_id;
	/* 0x0004 */	float						animation_interval_time;
	/* 0x0008 */	float						animation_time_threshold;
	/* 0x000c */	float						weight;
	/* 0x0010 */	u16							initial_event_types;
	/* 0x0014 */	animation_state const*		previous;
}; // struct animation_state_params

STATIC_SIZE_ASSERT(animation_state_params, 0x18);


} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_ANIMATION_STATE_H_INCLUDED