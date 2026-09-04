// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
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

	// claude@MATCH: the target writes each builder call as its OWN statement on
	// `main_lexeme_parameters` (each `lea &main_lexeme_parameters; call method`), NOT a
	// fluent chain - splitting them is what reproduces the per-line statement structure.
	animation::mixing::animation_lexeme_parameters main_lexeme_parameters( buffer, identifier, animation, NULL, NULL );
	main_lexeme_parameters.animated_object						( animated_object );
	main_lexeme_parameters.playback_type						( playback_type );
	main_lexeme_parameters.bones_mask							( animation::body_part_hands_only );
	main_lexeme_parameters.weight_synchronization_group_id		( offset_only_weight_synchronization_group_id );
	main_lexeme_parameters.weight_interpolator					( interpolator_for_offset_lexeme );
	main_lexeme_parameters.time_scale							( time_scale );
	main_lexeme_parameters.time_synchronization_group_id		( time_synchronization_group );
	// sushi@TODO: target line 40 here is a lone 4-byte `mov byte[ebp-5],0` (<0x4> at 0x72, no lea/call, no PDB local) - a compiler-materialized bool temp; do not fabricate a local. trail: patterns/lone-byte-store-zero.md
	// claude@NOTE: structural root cause - target records NO `main_lexeme` local (3 locals:
	//   main_lexeme_parameters, offset_lexeme, offset_lexeme_parameters), base records 4
	//   (adds main_lexeme). main_lexeme is a TEMPORARY in the target, yet it is referenced at
	//   line 48 (`group != -1 ? &main_lexeme : NULL`) and line 61 (weapon_lexeme_pair(...)) -
	//   the line-40 `mov byte[ebp-5],0` is the bool of an animation_lexeme(animation_lexeme&,
	//   bool) lifetime/clone idiom that keeps the unnamed temporary alive. Reproducing 0
	//   main_lexeme locals + the line-40 byte needs that exact idiom (unrecovered); rest of
	//   the byte residual is the animation-module inline builder-call expansions.
	animation::mixing::animation_lexeme main_lexeme( main_lexeme_parameters );


	// claude@MATCH: each builder call is its own statement (as the main chain above).
	// The 4th (time_driving_animation) ctor arg is the target's `push 0; mov edx,group;
	// sub edx,-1; neg; sbb edx,edx; lea &main_lexeme; and edx,eax` select ==
	// `group != u32(-1) ? &main_lexeme : NULL` (matches base byte-for-byte).
	animation::mixing::animation_lexeme_parameters offset_lexeme_parameters( buffer, identifier, animation, time_synchronization_group != u32( -1 ) ? &main_lexeme : NULL, NULL );
	offset_lexeme_parameters.animated_object					( animated_object );
	offset_lexeme_parameters.playback_type						( playback_type );
	offset_lexeme_parameters.start_animation_interval_id		( playback_state.interval_id );
	offset_lexeme_parameters.start_animation_interval_time		( playback_state.interval_time );
	offset_lexeme_parameters.bones_mask							( animation::body_part_whole_body_but_hands );
	offset_lexeme_parameters.weight_synchronization_group_id		( all_but_offset_weight_synchronization_group_id );
	if ( ! offset_lexeme_parameters.time_driving_animation( ) ) {
		ASSERT( UNKNOWN_EXPRESSION );

		offset_lexeme_parameters.time_scale( time_scale );
	}

	animation::mixing::animation_lexeme offset_lexeme( offset_lexeme_parameters ); return weapon_lexeme_pair( offset_lexeme, main_lexeme );
}

} // namespace survarium
