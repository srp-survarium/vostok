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

static char animation_intervals_channel_id[] = "anim_intervals";

// claude@NOTE: target is a fully optimized LTCG COMDAT (base is /Od), so the
// residual is overwhelmingly inline-vs-call: the target inlines the pinned_ptr
// ctor + the refcount-dance dtor, length_in_frames(), get_channel_id(),
// channel(), channel.knot(), the placement-new animation_interval ctor, and
// start_time()/length(); base CALLs every one. On top of that the target SOURCE
// structures the interval build differently from this faithful reconstruction:
// the single-interval fallback also covers channel->knots_count()==0 (here an
// assert), the per-knot loop carries NO in-body start_time()+length() check, and
// the final ("tail") interval is constructed AFTER the loop with the start-check
// applied to it (target records a `const volatile float tail` local). The
// optimized loop's bound/tail-index arithmetic does not resolve unambiguously
// from the asm, so the tail-after-loop shape is NOT reconstructed here to avoid a
// confidently-wrong structure; left at the faithful inside-loop-with-break form.
void animation_lexeme_parameters::create_animation_intervals( skeleton_animation_ptr const& animation )
{
	animation_interval* const animation_intervals	= static_cast<animation_interval*>( m_buffer.c_ptr() );
	R_ASSERT						( animation_intervals == m_animation_intervals );
	m_buffer						+= m_animation_intervals_count * sizeof(animation_interval);

	cubic_spline_skeleton_animation_pinned pinned_animation( animation );
	float const animation_length	= pinned_animation->length_in_frames()/default_fps;

	u32 const channel_id			= pinned_animation->event_channels().get_channel_id( animation_intervals_channel_id );
	if ( channel_id == u32(-1) ) {
#ifndef MASTER_GOLD
//		LOG_WARNING					( "animation [%s] has no animation intervals, assuming single interval", m_identifier );
#endif
		new ( animation_intervals ) animation_interval( animation, 0.f, animation_length );
		m_start_animation_interval_id	= 0;
		m_start_animation_interval_time	= 0.f;
		return;
	}

	event_channel const& channel	= pinned_animation->event_channels().channel( channel_id );

	R_ASSERT_CMP					( m_animation_intervals_count, ==, channel.domains_count() );
	R_ASSERT						( channel.knots_count() );
	float previous_knot				= channel.knot(0);
	for ( u32 knot_id = 1, knots_count = channel.knots_count(), interval_id = 0; knot_id < knots_count; ++knot_id, ++interval_id ) {
		float const knot			= channel.knot(knot_id);
		animation_interval* interval = animation_intervals + interval_id;
		new ( interval ) animation_interval( animation, previous_knot/default_fps, (knot - previous_knot)/default_fps );
		if ( interval->start_time() + interval->length() > animation_length ) {
			m_start_animation_interval_id	= interval_id;
			m_start_animation_interval_time	= animation_length - interval->start_time();
			R_ASSERT_CMP			( knot_id + 1, ==, knots_count );
			break;
		}
		previous_knot				= knot;
	}
}

// claude@NOTE: structure + locals faithful; residual is inline-vs-call from the
// target being an optimized COMDAT (base /Od). Target inlines the pinned_ptr ctor,
// event_channels() (a +8 offset, no operator void** call), channel() (imul 2Ch +
// member read of m_domains_count at +0x20), and the pinned_ptr destructor at each
// return path; base CALLs them. The two early/final returns ICF-share a `mov eax,1`
// + conditional `mov eax,domains_count` tail - a layout fold, not a source diff.
u32 animation_lexeme_parameters::animation_intervals_count	( skeleton_animation_ptr const& animation )
{
	cubic_spline_skeleton_animation_pinned pinned_animation( animation );

	u32 const channel_id			= pinned_animation->event_channels().get_channel_id( animation_intervals_channel_id );
	if ( channel_id == u32(-1) )
		return						1;

	event_channel const& channel	= pinned_animation->event_channels().channel( channel_id );
	return							channel.domains_count( );
}

// claude@NOTE: structure matches the target's local set (pinned_animation,
// animation_length, const start_time - no `length` local) and the two-return /
// single-merged-ctor shape (both `return animation_interval(...)` share one ctor
// site). Residual: (1) the target out-of-lines `animation_interval::animation_interval`
// while our base inlines it (intrusive_ptr::set + the three field stores) - an
// inline-threshold/LTCG effect, not source-steerable; (2) the target materializes
// `channel_id != u32(-1)` as a standalone statement (`setne`, its own source line)
// ahead of the channel resolution - a condition-split quirk no faithful spelling
// reproduces without a phantom bool local the target does not record.
vostok::animation::mixing::animation_interval animation_lexeme_parameters::create_animation_interval( skeleton_animation_ptr const& animation, const u32 interval_id )
{
	cubic_spline_skeleton_animation_pinned pinned_animation( animation );
	float const animation_length	= pinned_animation->length_in_frames()/default_fps;

	u32 const channel_id			= pinned_animation->event_channels().get_channel_id( animation_intervals_channel_id );

	event_channel const* const channel	= ( channel_id != u32(-1) ) ? &pinned_animation->event_channels().channel( channel_id ) : 0;
	if ( channel && channel->knots_count() ) {
		float const start_time			= channel->knot( interval_id )/default_fps;
		return						animation_interval( animation, start_time, ( interval_id + 1 < channel->knots_count() ) ?
			channel->knot( interval_id + 1 )/default_fps - start_time :
			animation_length - start_time );
	}

	return							animation_interval( animation, 0.f, animation_length );
}