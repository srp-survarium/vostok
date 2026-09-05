////////////////////////////////////////////////////////////////////////////
//	Created		: 17.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_animation_state.h"
#include "mixing_n_ary_tree_animation_node.h"

using vostok::animation::mixing::animation_state;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::skeleton_animation_ptr;
using vostok::animation::subscribed_channel;

animation_state::animation_state	(
	n_ary_tree_animation_node&		animation_node,
	const u32						time_in_ms,
	const u16						initial_event_types,
	const u32						animation_interval_id,
	const u32						previous_animation_interval_id,
	const float						animation_interval_time,
	const float						animation_time_threshold,
	const float						weight,
	subscribed_channel*&			channels_head,
	const bool						is_freezed
	) :
	animation_interval_id			( animation_interval_id ),
	previous_animation_interval_id	( previous_animation_interval_id ),
	animation_interval_time			( animation_interval_time ),
	animation_time					( animation_node.animation_intervals()[ animation_interval_id ].start_time() + animation_interval_time ),
	animation_time_threshold		( animation_time_threshold ),
	event_iterator					( get_this(), animation_node, time_in_ms, initial_event_types, channels_head ),
	weight							( weight ),
	are_there_any_weight_transitions( false ),
	is_freezed						( is_freezed )
{
	float const animation_length	= cubic_spline_skeleton_animation_pinned(animation_node.animation_intervals()[ animation_interval_id ].animation())->length_in_frames()/default_fps;
	if ( animation_node.playback_type( ) == vostok::animation::play_cyclically && animation_time >= animation_length ) {
		animation_time				-= animation_length;
		this->animation_time_threshold	= animation_length;
	}
}
