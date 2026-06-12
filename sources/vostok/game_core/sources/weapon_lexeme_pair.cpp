////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

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
	u32											time_synchronization_group,
	float										time_scale,
	animation::mixing::playback_enum			playback_type,
	animation::base_interpolator const&			interpolator_for_offset_lexeme
)
{
	enum {
		all_but_offset_weight_synchronization_group_id	= 256,
		offset_only_weight_synchronization_group_id		= 128,
	};

	animation::mixing::animation_lexeme_parameters main_lexeme_parameters( buffer, identifier, animation, NULL, NULL );
	main_lexeme_parameters
		.animated_object						( animated_object )
		.playback_type							( playback_type )
		.bones_mask								( animation::body_part_hands_only )
		.weight_synchronization_group_id		( offset_only_weight_synchronization_group_id )
		.weight_interpolator					( interpolator_for_offset_lexeme )
		.time_scale								( time_scale )
		.time_synchronization_group_id			( time_synchronization_group );

	// sushi@TODO: target has a lone 4-byte `mov byte[ebp-5],0` here (<0x4> at 0x72, no lea/call,
	// no PDB local). NOT an unused `bool` (this project has ~no unused variables, and the PDB
	// records no such local) - likeliest a compiler-MATERIALIZED bool temp (a `false` bound into
	// the parameter chain / a bool const& site) or a detached first record of a split eater.
	// Origin unidentified; do not fabricate a local for it. trail: patterns/lone-byte-store-zero.md
	animation::mixing::animation_lexeme main_lexeme( main_lexeme_parameters );

	// claude@MATCH: the 4th (time_driving_animation) arg is the target's
	// `push 0; mov edx,group; sub edx,-1; neg; sbb edx,edx; lea &main_lexeme; and edx,eax`
	// select == `group != u32(-1) ? &main_lexeme : NULL` (matches base byte-for-byte).
	animation::mixing::animation_lexeme_parameters offset_lexeme_parameters(
		buffer,
		identifier,
		animation,
		time_synchronization_group != u32( -1 ) ? &main_lexeme : NULL,
		NULL
	);
	offset_lexeme_parameters
		.animated_object						( animated_object )
		.playback_type							( playback_type )
		.start_animation_interval_id			( playback_state.interval_id )
		.start_animation_interval_time			( playback_state.interval_time )
		.bones_mask								( animation::body_part_whole_body_but_hands )
		.weight_synchronization_group_id		( all_but_offset_weight_synchronization_group_id );

	if ( ! offset_lexeme_parameters.time_driving_animation( ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		offset_lexeme_parameters.time_scale( time_scale );
	}

	animation::mixing::animation_lexeme offset_lexeme( offset_lexeme_parameters );

	return weapon_lexeme_pair( offset_lexeme, main_lexeme );
}

} // namespace survarium
