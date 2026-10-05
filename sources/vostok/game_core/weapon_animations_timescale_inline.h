// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED
#define WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED

#include <vostok/animation/cubic_spline_skeleton_animation.h>

namespace survarium {

inline float computed_reload_animation_time_scale( resources::managed_resource_ptr const& reload_animation, const float reload_time )
{
	float const reload_animation_time	= resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( reload_animation )->length_in_frames( ) * (1.f / animation::default_fps);
	return reload_animation_time / reload_time;
}

// claude@NOTE: the scanned domain byte is compared against a bare 7 because the target has no
// enum for it - event_channel::domain_data is a single u8 and channel_type_enum only goes to 2,
// so 7 is a wire value with no recoverable name. The loop bound really is knots_count(), not
// domains_count() (which is knots_count()-1) - the target's `cmp eax, ecx` uses [channel+0x20]
// (time_channel::m_knots_count) directly.
inline float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second )
{
	resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation > pinned_animation( shooting_animation );
	animation::animation_event_channels const&	channels	= pinned_animation->event_channels( );

	u32											channel_id	= 0;
	animation::event_channel const*				channel		= & channels.channel( channel_id );
	while ( vostok::strings::compare( channel->name( ), "shoot" ) )
		channel											= & channels.channel( ++channel_id );

	u32											events_count	= 0;
	for ( u32 i = 0; i < channel->knots_count( ); ++i )
		if ( channel->domain( i ).data == 7 )
			++events_count;

	float const									animation_time	= pinned_animation->length_in_frames( ) * (1.f / animation::default_fps);

	return													rounds_per_second / ( events_count / animation_time );
}

inline float computed_shotgun_reload_animation_time_scale( resources::managed_resource_ptr const& reload_one_round_anim, u32 const magazine_capacity, float const reload_time )
{
	float const one_round_animation_time	= resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( reload_one_round_anim )->length_in_frames( ) * (1.f / animation::default_fps);
	return one_round_animation_time * magazine_capacity / reload_time;
}

} // namespace survarium

#endif // #ifndef WEAPON_ANIMATIONS_TIMESCALE_INLINE_H_INCLUDED
