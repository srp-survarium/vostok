// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "bi_spline_skeleton_animation_baked.h"

namespace vostok {
namespace animation {

void create_baked_animation_in_place( void* const raw_buffer, const u32 buffer_size )
{
	mutable_buffer buffer( raw_buffer, buffer_size );
	bi_spline_skeleton_animation_baked_impl const* const impl =
		static_cast< bi_spline_skeleton_animation_baked_impl const* >( buffer.c_ptr() );
	buffer += sizeof( *impl );

	bi_spline_bone_animation_baked* const bones =
		static_cast< bi_spline_bone_animation_baked* >( buffer.c_ptr() );
	buffer += impl->bones_count * sizeof( *bones );

	bi_spline_event_channel_baked* const event_channels =
		static_cast< bi_spline_event_channel_baked* >( buffer.c_ptr() );
	buffer += impl->event_channels_count * sizeof( *event_channels );

	bi_spline_bone_animation_baked* bone = bones;
	bi_spline_bone_animation_baked* const bones_end = bones + impl->bones_count;
	for ( ; bone != bones_end; ++bone )
	{
		typedef platform_pointer_selector<
			bi_spline_channel_animation_baked,
			platform_pointer_64bit
		>::helper bi_spline_channel_animation_baked_ptr;

		bi_spline_channel_animation_baked_ptr* channel = bone->m_channel_animations;
		bi_spline_channel_animation_baked_ptr* const channels_end = channel + channel_max;
		for ( ; channel != channels_end; ++channel )
		{
			bi_spline_channel_animation_baked* const channel_animation =
				static_cast< bi_spline_channel_animation_baked* >( buffer.c_ptr() );
			*channel = channel_animation;

			buffer += sizeof( *channel_animation );
			buffer += channel_animation->knots_count() * sizeof( std::pair< float, float > );
		}
	}

	for ( u32 channel = 0; channel < impl->event_channels_count; ++channel )
	{
		bi_spline_event_channel_baked& event_channel = event_channels[channel];
		event_channel.m_knots = static_cast< float* >( buffer.c_ptr() );

		buffer += event_channel.knots_count() * sizeof( float );
		buffer += math::align_up(
			event_channel.domains_count() + strings::length( event_channel.name() ) + 1,
			u32( 4 )
		);
	}
}

} // namespace animation
} // namespace vostok
