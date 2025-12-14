////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED
#define WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
float computed_reload_animation_time_scale( resources::managed_resource_ptr const& reload_animation, float reload_time )
{
	return 0.0f;

	// FUNCTION BODY
	// <0xac9a4>|0x004|+0x085:'17'
	// <0xaca29>|0x089|+0x006:'18'
	// ******
}

// STATE[STUB]
float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second )
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

// STATE[STUB]
float computed_shotgun_reload_animation_time_scale( resources::managed_resource_ptr const& reload_one_round_anim, u32 magazine_capacity, float reload_time )
{
	return 0.0f;

	// FUNCTION BODY
	// <0xacb84>|0x004|+0x06d:'52'
	// <0xacbf1>|0x071|+0x01e:'53'
	// ******
}

} // namespace survarium

#endif // #ifndef WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED
