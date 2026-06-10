////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_lexeme_pair.h>

#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

// STATE[61.19%|PARTIAL]: now PAIRED (was None; the current objdiff/delinker pairs the
// character-identical mangled names - the playback_enum tag fix from PR #155 holds). The
// residual is the documented whole-program inline-vs-call of the trivial inline-in-class
// animation_lexeme_parameters setters: target keeps animated_object/playback_type/bones_mask/
// weight_interpolator/start_animation_interval_id out-of-line, our /GL inlines them (~0x19f
// vs target 0x1f7 bytes). Inline COMDATs in the out-of-scope `animation` headers; the only
// lever (move them out-of-line) is engine-wide. Same unsteerable class as scheduler::on_frame.
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

	// claude@MATCH: L40 is a lone 4-byte `mov byte[ebp-5],0` dead store (target <0x4> at 0x72,
	// NO following lea/call) sitting between the last main_lexeme_parameters setter and the
	// main_lexeme construction - an unused `bool` local dead-stored under /Od, NOT an ASSERT (an
	// ASSERT emits the byte-store PLUS lea+call = <0xc>). The reviewer's doubt ("dummy isn't in
	// the locals") is consistent with a never-read bool: MSVC's PDB local table can omit a
	// variable whose sole use is a dead store, yet /Od still allocates its slot and emits this
	// store. The single statement's byte shape is reproduced exactly; only the whole-function
	// pairing is blocked (None) by the setter inline-vs-call (see STATE), independent of this line.
	bool dummy = false;
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

	// STRUCTURE DIFF: target 21 stmts / base 11 stmts
	// SIZE +0x1  | 42 | animation::mixing::animation_lexeme_parameters main_lexeme_parameters( buffer, identifier, animation, NULL, NULL );
	// SIZE +0x4b | 50 | 		.time_synchronization_group_id			( time_synchronization_group );
	// SIZE -0x4  | 60 | bool dummy = false;
	// BASE_ONLY  | 61 | animation::mixing::animation_lexeme main_lexeme( main_lexeme_parameters );
	// BASE_ONLY  | 72 | );
	// BASE_ONLY  | 79 | 		.weight_synchronization_group_id		( all_but_offset_weight_synchronization_group_id );
	// BASE_ONLY  | 81 | if ( ! offset_lexeme_parameters.time_driving_animation( ) )
	// BASE_ONLY  | 83 | 		ASSERT( UNKNOWN_EXPRESSION );
	// TRGT_ONLY x14 (the per-line setter/statement records of both chains)
	// SIZE +0x35 | 89 | return weapon_lexeme_pair( offset_lexeme, main_lexeme );
	// VERDICT: STRUCTURE MATCH (shape ok) - the source statements are 1:1; the 21-vs-11 record
	// count is a line-table artifact of the setter inline-vs-call wall (target's out-of-line
	// setter CALLS keep one record per chain line; our inlined chains collapse onto each
	// statement's anchor line). The dummy SIZE -0x4 is disp8->disp32 slot noise from the
	// larger inlined frame. Non-steerable from this TU.
}

} // namespace survarium
