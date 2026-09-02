////////////////////////////////////////////////////////////////////////////
//	Created		: 28.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/cubic_spline_skeleton_animation.h>

using vostok::animation::mixing::animation_lexeme_parameters;
using vostok::animation::skeleton_animation_ptr;

static char const animation_intervals_channel_id[] = "anim_intervals";

void animation_lexeme_parameters::create_animation_intervals( skeleton_animation_ptr const& animation )
{
	animation_interval* const animation_intervals	= static_cast<animation_interval*>( m_buffer.c_ptr() );
	R_ASSERT						( animation_intervals == m_animation_intervals );
	m_buffer						+= m_animation_intervals_count * sizeof(animation_interval);

	cubic_spline_skeleton_animation_pinned pinned_animation( animation );
	float const animation_length	= pinned_animation->length_in_frames()/default_fps;

	u32 const channel_id			= pinned_animation->event_channels().get_channel_id( animation_intervals_channel_id );
	bool const channel_id_exists	= channel_id != u32(-1);

	event_channel const* const channel = channel_id_exists ? &pinned_animation->event_channels().channel( channel_id ) : 0;
	if ( !channel || !channel->knots_count() ) {






		new ( animation_intervals ) animation_interval( animation, 0.f, animation_length );

		m_start_animation_interval_id	= 0;
		m_start_animation_interval_time	= 0.f;
		return;
	}

	u32 const knots_count			= channel->knots_count();
	float previous_knot				= channel->knot(0);
	for ( u32 knot_id = 1; knot_id < knots_count; ++knot_id ) {
		float const knot			= channel->knot(knot_id);
		animation_interval* const interval = animation_intervals + knot_id - 1;
		new ( interval ) animation_interval( animation, previous_knot/default_fps, (knot - previous_knot)/default_fps );
		previous_knot				= knot;
	}


	u32 const animation_interval_id = knots_count - 1;
	animation_interval* const interval = animation_intervals + animation_interval_id;
	float const volatile tail		= (channel->knot(0) - previous_knot)/default_fps;
	new ( interval ) animation_interval( animation, previous_knot/default_fps, animation_length + tail );





	float const volatile animation_interval_end = interval->start_time() + interval->length();
	if ( animation_interval_end > animation_length ) {
		m_start_animation_interval_id	= animation_interval_id;
		m_start_animation_interval_time	= animation_length - interval->start_time();
	}
}

u32 animation_lexeme_parameters::animation_intervals_count	( skeleton_animation_ptr const& animation )
{
	cubic_spline_skeleton_animation_pinned pinned_animation( animation );

	u32 const channel_id			= pinned_animation->event_channels().get_channel_id( animation_intervals_channel_id );
	if ( channel_id == u32(-1) )
		return						1;

	event_channel const& channel	= pinned_animation->event_channels().channel( channel_id );
	if ( !channel.knots_count() )
		return						1;

	return							channel.knots_count( );
}






vostok::animation::mixing::animation_interval animation_lexeme_parameters::create_animation_interval( skeleton_animation_ptr const& animation, const u32 interval_id )
{
	cubic_spline_skeleton_animation_pinned pinned_animation( animation );
	float const animation_length	= pinned_animation->length_in_frames()/default_fps;

	u32 const channel_id			= pinned_animation->event_channels().get_channel_id( animation_intervals_channel_id );
	bool const channel_id_exists	= channel_id != u32(-1);

	event_channel const* const channel	= channel_id_exists ? &pinned_animation->event_channels().channel( channel_id ) : 0;
	if ( !channel || !channel->knots_count() )



		return						animation_interval( animation, 0.f, animation_length );




	float const start_time			= channel->knot( interval_id )/default_fps;
	return							animation_interval( animation, start_time, ( interval_id + 1 < channel->knots_count() ) ?
		channel->knot( interval_id + 1 )/default_fps - start_time :
		animation_length - start_time );

}
