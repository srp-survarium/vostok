////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_lexeme_pair.h>

#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

// STATE[None|PARTIAL]: body is a faithful 1:1 reconstruction (every statement, constant,
// control-flow branch and the return arg order are byte-exact where not inlined). The sole
// residual is the whole-program inline-vs-call of the trivial inline-in-class
// animation_lexeme_parameters setters: the TARGET keeps animated_object/playback_type/
// bones_mask/weight_interpolator/start_animation_interval_id out-of-line (real `call`s), our
// /GL LTCG inlines them at this site (and folds ~animation_lexeme_parameters to the empty-fn).
// That shifts the whole [ebp-XX] layout and shortens the body enough that objdiff can no longer
// pair it (-> None). The setters live in the out-of-scope `animation` module's headers as
// inline COMDATs; moving them out-of-line (the only known lever) is engine-wide and out of
// scope. Same unsteerable inline-vs-call class as scheduler::on_frame / operator| / fixed_string
// in assembly_patterns.md. See get_weapon_lexeme_pair_impl.md.
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
		// sushi@TODO: is 2 a bare magic constant or a named value defined somewhere?
		.bones_mask								( 2 )
		.weight_synchronization_group_id		( offset_only_weight_synchronization_group_id )
		.weight_interpolator					( interpolator_for_offset_lexeme )
		.time_scale								( time_scale )
		.time_synchronization_group_id			( time_synchronization_group );

	// claude@MATCH: L40 is a lone 4-byte `mov byte[ebp-N],0` dead store (target <0x4>, NO
	// lea/call) - an unused bool local, NOT an ASSERT (an ASSERT would emit lea+call = <0xc>).
	// sushi@TODO: unlikely a `bool dummy` - if it were, we'd have seen `dummy` in the locals
	// (we didn't). Needs further matching to recover the compiled-out structure here.
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

	// FUNCTION BODY (target rva 0x7a13e0; carcass VAs are base-build +0x10000)
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7b13eb>|0x00b|+0x014:'32'	animation_lexeme_parameters main_lexeme_parameters( buffer, identifier, animation, NULL, NULL );
	// <0x7b13ff>|0x01f|+0x00b:'33'	.animated_object( animated_object )
	// <0x7b140a>|0x02a|+0x00b:'34'	.playback_type( playback_type )
	// <0x7b1415>|0x035|+0x00d:'35'	.bones_mask( 2 )
	// <0x7b1422>|0x042|+0x00d:'36'	.weight_synchronization_group_id( offset_only... = 0x80 )
	// <0x7b142f>|0x04f|+0x00b:'37'	.weight_interpolator( interpolator_for_offset_lexeme )
	// <0x7b143a>|0x05a|+0x00d:'38'	.time_scale( time_scale )
	// <0x7b1447>|0x067|+0x00b:'39'	.time_synchronization_group_id( time_synchronization_group )
	// <0x7b1452>|0x072|+0x004:'40'	bool dummy = false;  (lone 4-byte dead store)
	// <0x7b1456>|0x076|+0x00e:'41'	animation_lexeme main_lexeme( main_lexeme_parameters );
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7b1464>|0x084|+0x028:'48'	animation_lexeme_parameters offset_lexeme_parameters( buffer, identifier, animation, group!=-1?&main:NULL, NULL );
	// <0x7b148c>|0x0ac|+0x00e:'49'	.animated_object( animated_object )
	// <0x7b149a>|0x0ba|+0x00e:'50'	.playback_type( playback_type )
	// <0x7b14a8>|0x0c8|+0x029:'51'	.start_animation_interval_id( playback_state.interval_id )  (R_ASSERT_CMP -> 1 empty_stub)
	// <0x7b14d1>|0x0f1|+0x044:'52'	.start_animation_interval_time( playback_state.interval_time )  (R_ASSERT_CMP x2)
	// <0x7b1515>|0x135|+0x010:'53'	.bones_mask( body_part_whole_body_but_hands = -3 )
	// <0x7b1525>|0x145|+0x010:'54'	.weight_synchronization_group_id( all_but_offset... = 0x100 )
	// <0x7b1535>|0x155|+0x015:'55'	if ( ! offset_lexeme_parameters.time_driving_animation( ) )
	// <0x7b154a>|0x16a|+0x012:'56'	ASSERT( UNKNOWN_EXPRESSION );
	// <0>
	// <0x7b155c>|0x17c|+0x010:'58'	offset_lexeme_parameters.time_scale( time_scale );
	// <0>
	// <1>
	// <0x7b156c>|0x18c|+0x065:'61'	return weapon_lexeme_pair( offset_lexeme, main_lexeme );
	// ******
}

} // namespace survarium
