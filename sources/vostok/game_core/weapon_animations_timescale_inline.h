////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED
#define WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED

#include <vostok/animation/cubic_spline_skeleton_animation.h>

namespace survarium {

// STATE[STUB]
inline float computed_reload_animation_time_scale( resources::managed_resource_ptr const& reload_animation, float reload_time )
{
	return 0.0f;

	// FUNCTION BODY
	// <0xac9a4>|0x004|+0x085:'17'
	// <0xaca29>|0x089|+0x006:'18'
	// ******
}

// STATE[STUB]
inline float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second )
{
	// LOCALS
	// resources::pinned_ptr_const<animation::cubic_spline_skeleton_animation> pinned_animation
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0xaca47>|0x007|+0x031:'23'
	// <0xaca78>|0x038|+0x007:'24'
	// <0>
	// <0xaca7f>|0x03f|+0x002:'26'
	// <0xaca81>|0x041|+0x008:'27'
	// <0xaca89>|0x049|+0x039:'28'
	// <0>
	// <1>
	// <0xacac2>|0x082|+0x002:'31'
	// <0xacac4>|0x084|+0x014:'32'
	// <0xacad8>|0x098|-0x00e:'32'
	// <0xacaca>|0x08a|+0x00d:'33'
	// <0xacad7>|0x097|+0x006:'34'
	// <0>
	// <1>
	// <2>
	// <0xacadd>|0x09d|+0x009:'38'
	// <0>
	// <1>
	// <0xacae6>|0x0a6|+0x08c:'41'
	// <0>
	// <1>
	// <2>
	// ******
}

// claude@NOTE: target emits this COMDAT with x87 FP + length_in_frames() fully inlined;
// our base picks SSE (divss) and out-of-lines length_in_frames - a COMDAT-codegen/FP-flag
// difference owned by whichever TU wins the COMDAT, not steerable from this body.
inline float computed_shotgun_reload_animation_time_scale( resources::managed_resource_ptr const& reload_one_round_anim, u32 const magazine_capacity, float const reload_time )
{
	float const one_round_animation_time	= resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( reload_one_round_anim )->length_in_frames( ) / 30.f;
	return one_round_animation_time * magazine_capacity / reload_time;
}

} // namespace survarium

#endif // #ifndef WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED
