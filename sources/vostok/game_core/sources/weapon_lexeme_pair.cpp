// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_lexeme_pair.h>

#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

weapon_lexeme_pair get_weapon_lexeme_pair_impl(
	mutable_buffer&								buffer,
	pcstr										identifier,
	resources::managed_resource_ptr const&		animation,
	pcvoid										animated_object,
	animation::animation_playback_state const&	playback_state,
	const u32									time_synchronization_group,
	const float									time_scale,
	const animation::mixing::playback_enum		playback_type,
	animation::base_interpolator const&			interpolator_for_offset_lexeme
)
{
	enum {
		all_but_offset_weight_synchronization_group_id	= 256,
		offset_only_weight_synchronization_group_id		= 128,
	};

	// each builder call is its own statement: retail records one line per call, not a chain
	animation::mixing::animation_lexeme_parameters offset_lexeme_parameters( buffer, identifier, animation, NULL, NULL );
	offset_lexeme_parameters.animated_object						( animated_object );
	offset_lexeme_parameters.playback_type							( playback_type );
	offset_lexeme_parameters.bones_mask								( animation::body_part_hands_only );
	offset_lexeme_parameters.weight_synchronization_group_id		( offset_only_weight_synchronization_group_id );
	offset_lexeme_parameters.weight_interpolator					( interpolator_for_offset_lexeme );
	offset_lexeme_parameters.time_scale								( time_scale );
	offset_lexeme_parameters.time_synchronization_group_id			( time_synchronization_group );
	// sushi@TODO: target line 40 here is a lone 4-byte `mov byte[ebp-5],0` (<0x4> at 0x72, no lea/call, no PDB local) - a compiler-materialized bool temp; do not fabricate a local. trail: patterns/lone-byte-store-zero.md
	animation::mixing::animation_lexeme offset_lexeme( offset_lexeme_parameters );

	animation::mixing::animation_lexeme_parameters main_lexeme_parameters( buffer, identifier, animation, time_synchronization_group != u32( -1 ) ? &offset_lexeme : NULL, NULL );
	main_lexeme_parameters.animated_object						( animated_object );
	main_lexeme_parameters.playback_type						( playback_type );
	main_lexeme_parameters.start_animation_interval_id			( playback_state.interval_id );
	main_lexeme_parameters.start_animation_interval_time		( playback_state.interval_time );
	main_lexeme_parameters.bones_mask							( animation::body_part_whole_body_but_hands );
	main_lexeme_parameters.weight_synchronization_group_id		( all_but_offset_weight_synchronization_group_id );
	if ( ! main_lexeme_parameters.time_driving_animation( ) ) {
		ASSERT( UNKNOWN_EXPRESSION );

		main_lexeme_parameters.time_scale( time_scale );
	}

	return weapon_lexeme_pair( animation::mixing::animation_lexeme( main_lexeme_parameters ), offset_lexeme );
}

} // namespace survarium
