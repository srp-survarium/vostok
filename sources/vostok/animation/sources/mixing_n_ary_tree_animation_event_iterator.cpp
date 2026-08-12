////////////////////////////////////////////////////////////////////////////
//	Created		: 26.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_animation_event_iterator.h"
#include "mixing_n_ary_tree_time_in_ms_calculator.h"
#include <vostok/animation/mixing_animation_interval.h>
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_animation_state.h"
#include <vostok/animation/animation_callback.h>

using vostok::animation::mixing::n_ary_tree_animation_event_iterator;
using vostok::animation::mixing::animation_interval;
using vostok::animation::mixing::animation_state;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::mixing::channel_event_callback_base;
using vostok::animation::subscribed_channel;

void n_ary_tree_animation_event_iterator::invert_times( u32 const time_in_ms )
{
	if ( m_value.event_type )
		m_value.event_time_in_ms	= time_in_ms - m_value.event_time_in_ms;
}

n_ary_tree_animation_event_iterator::n_ary_tree_animation_event_iterator	(
		animation_state&				animation_state,
		n_ary_tree_animation_node&		animation_node,
		u32								start_time_in_ms,
		u16								event_types,
		u8								channel_ids,
		subscribed_channel*&			channels_head

	) :
	m_animation							( &animation_node ),
	m_channels_head						( &channels_head ),
	m_value								(
		u8( -1 ),
		animation_state.animation_interval_id,
		animation_state.animation_interval_time,
		start_time_in_ms,
		event_types,
		0
	)
{
	advance								( event_types );
}

float n_ary_tree_animation_event_iterator::get_nearest_animation_interval_event_time	(
		animation_interval const& interval,
		float const start_time,
		float target_time,
		u16& event_type,
		u8&								channel_ids,
		u8&								domain_data,
		bool							start_time_may_be_used
	)
{
	channel_ids						= 0;
	domain_data						= u8( -1 );
	int const time_direction			= start_time <= target_time ? 1 : -1;

	cubic_spline_skeleton_animation_pinned pinned_animation( interval.animation() );

	event_type							= time_event_animation_interval_ended;
	float const animation_length		= pinned_animation->length_in_frames() / default_fps;
	float const animation_end_time		= animation_length - interval.start_time();
	if ( start_time_may_be_used ?
		 time_direction*(interval.start_time() + start_time) <= time_direction*animation_length :
		 time_direction*(interval.start_time() + start_time) < time_direction*animation_length ) {
		if ( time_direction*(interval.start_time() + target_time) >= time_direction*animation_length ) {
			if ( interval.start_time() + target_time == animation_length )
				event_type				|= time_direction == 1 ? time_event_animation_ended_in_positive_direction : time_event_animation_ended_in_negative_direction;
			else
				event_type				= time_direction == 1 ? time_event_animation_ended_in_positive_direction : time_event_animation_ended_in_negative_direction;

			target_time					= animation_end_time;
		}
	}
	u32 const channels_count			= pinned_animation->event_channels().channels_count();
	if ( !channels_count )
		return							target_time;

	for ( subscribed_channel const* subscribed_channel = *m_channels_head; subscribed_channel; subscribed_channel = subscribed_channel->next ) {
		u32 const channel_id			= pinned_animation->event_channels().get_channel_id( subscribed_channel->channel_id );
		if ( channel_id == u32(-1) )
			continue;

		event_channel const& channel	= pinned_animation->event_channels().channel( channel_id );
		u32 const knots_count			= channel.knots_count( );
		float const* current_knot		= time_direction == 1 ? channel.knots() : (channel.knots() + knots_count - 1);
		float const* const knots_end	= time_direction == 1 ? (channel.knots() + knots_count) : (channel.knots() - 1);
		for ( ; current_knot != knots_end; current_knot += time_direction ) {
			float current_knot_time		= (*current_knot) / default_fps;
			if ( animation_length <= current_knot_time )
				current_knot_time		-= animation_length;

			current_knot_time			-= interval.start_time();
			if ( current_knot_time < 0.f ) {
				current_knot_time		+= animation_length;
				if ( current_knot_time*time_direction > target_time*time_direction &&
					 !math::is_relatively_similar( current_knot_time*time_direction, target_time*time_direction, math::epsilon_5 ) )
					continue;
			}

			if ( start_time_may_be_used ?
				 current_knot_time*time_direction < start_time*time_direction :
				 current_knot_time*time_direction <= start_time*time_direction ||
				 math::is_relatively_similar( current_knot_time*time_direction, start_time*time_direction, math::epsilon_5 ) ) {
				current_knot_time		+= animation_length;
				if ( current_knot_time*time_direction >= target_time*time_direction )
					continue;
			}

			if ( current_knot_time*time_direction > target_time*time_direction &&
				 !math::is_relatively_similar( current_knot_time*time_direction, target_time*time_direction, math::epsilon_5 ) )
				continue;

			u8 const channel_bit		= u8( 1 ) << channel_id;
			if ( current_knot_time == target_time ) {
				event_type				|= time_event_channel_callback_should_be_fired;
				channel_ids				|= channel_bit;
			}
			else {
				event_type				= time_event_channel_callback_should_be_fired;
				channel_ids				= channel_bit;
			}

			if ( channel.type() != channel_type_events )
				domain_data				= channel.domain( u32( current_knot - channel.knots() ) % knots_count ).data;
			else
				domain_data				= u8( -1 );
			target_time					= current_knot_time;
			break;
		}
	}

	return								target_time;
}

u32 n_ary_tree_animation_event_iterator::get_time_in_ms				(
		u32 const start_time_in_ms,
		float const time_from_interval_start,
		float& event_time,
		u16& event_type
	)
{
	vostok::animation::mixing::n_ary_tree_time_in_ms_calculator time_in_ms_calculator(
		*(m_animation->time_driving_animation() ? m_animation->time_driving_animation() : m_animation), // sushi@TODO
		start_time_in_ms,
		time_from_interval_start,
		event_time,
		event_type
	);
	u32 const result					= time_in_ms_calculator.time_in_ms( );
	R_ASSERT_CMP						( start_time_in_ms, <=, result );
	event_type							= time_in_ms_calculator.event_type( );
	event_time							= time_in_ms_calculator.event_time( );
	return								result;
}

void n_ary_tree_animation_event_iterator::advance					( u16 const initial_event_types )
{
	bool const start_time_may_be_used	= !!(initial_event_types & time_event_animation_lexeme_started);
	if ( !initial_event_types && (m_animation->playback_type() == play_once_and_freeze_at_end) ) {
		animation_state const* const state = m_animation->get_animation_state();
		if ( (state && state->is_freezed) || (m_value.event_type & time_event_animation_ended_in_positive_direction) ) {
			m_value						= animation_event( u32(-1), 0, 0 );
			m_animation					= 0;
			return;
		}
	}

	if ( m_animation->time_calculator() && !start_time_may_be_used ) {
		m_value							= animation_event( u32(-1), 0, 0 );
		m_animation						= 0;
		return;
	}

	float animation_state_interval_time =
		m_animation->time_driving_animation() ?
		m_animation->time_driving_animation()->animation_state().animation_interval_time :
		m_value.animation_interval_time;
	u32 iteration						= 0;
	for ( ;; ++iteration ) {
		animation_interval const* current_interval	= m_animation->animation_intervals() + m_value.animation_interval_id;
		if ( current_interval->length() < m_value.animation_interval_time )
			m_value.animation_interval_time	= current_interval->length();

		if ( iteration )
			animation_state_interval_time	= m_value.animation_interval_time;

		n_ary_tree_animation_node& driving_animation =
			*(m_animation->time_driving_animation() ? m_animation->time_driving_animation() : m_animation);
		float time_scale;
		if ( driving_animation.operands_count() &&
			 (*driving_animation.operands( sizeof(n_ary_tree_animation_node) ))->is_time_scale() ) {
			vostok::animation::mixing::n_ary_tree_time_scale_calculator time_scale_calculator(
				m_value.event_time_in_ms,
				m_value.animation_interval_time,
				m_value.event_time_in_ms
			);
			(*driving_animation.operands( sizeof(n_ary_tree_animation_node) ))->accept( time_scale_calculator );
			time_scale					= time_scale_calculator.time_scale();
		}
		else
			time_scale					= 1.f;

		if ( !initial_event_types && (time_scale == 0.f) ) {
			vostok::animation::mixing::n_ary_tree_time_scale_calculator time_scale_calculator(
				m_value.event_time_in_ms + 1,
				m_value.animation_interval_time,
				m_value.event_time_in_ms + 1
			);
			n_ary_tree_base_node* const time_scale_node =
				driving_animation.operands_count() ?
				*driving_animation.operands( sizeof(n_ary_tree_animation_node) ) :
				0;
			if ( time_scale_node && time_scale_node->is_time_scale() ) {
				time_scale_node->accept	( time_scale_calculator );
				time_scale				= time_scale_calculator.time_scale();
			}
			else
				time_scale				= 1.f;

			if ( time_scale == 0.f ) {
				m_value					= animation_event( u32(-1), 0, 0 );
				m_animation				= 0;
				return;
			}
		}

		animation_interval const* current_driving_interval =
			driving_animation.animation_intervals() + m_value.animation_interval_id;
		float const driving_animation_factor = current_driving_interval->length() / current_interval->length();
		m_value.event_type				= initial_event_types;
		animation_event value			= m_value;
		u16 event_type;
		if ( time_scale >= 0.f ) {
			if ( animation_state_interval_time > m_value.animation_interval_time )
				animation_state_interval_time = m_value.animation_interval_time;

			float new_event_time			= get_nearest_animation_interval_event_time(
				*current_interval,
				m_value.animation_interval_time,
				current_interval->length(),
				event_type,
				m_value.channel_ids,
				m_value.domain_data,
				start_time_may_be_used
			) * driving_animation_factor;
			u32 const new_event_time_in_ms = get_time_in_ms(
				m_value.event_time_in_ms,
				animation_state_interval_time*driving_animation_factor,
				new_event_time,
				event_type
			);
			if ( initial_event_types && (new_event_time_in_ms != m_value.event_time_in_ms) )
				return;

			m_value.event_type |= event_type, new_event_time /= driving_animation_factor, m_value.event_time_in_ms = new_event_time_in_ms, m_value.animation_interval_time = new_event_time;

			if ( m_value.event_type & time_event_animation_interval_ended ) {
				if ( ++m_value.animation_interval_id == m_animation->animation_intervals_count() )
					m_value.animation_interval_id = m_animation->start_cycle_animation_interval_id();
				m_value.animation_interval_time = 0.f;
			}
		}
		else {
			if ( m_value.animation_interval_time > animation_state_interval_time )
				animation_state_interval_time = m_value.animation_interval_time;

			float new_event_time			= get_nearest_animation_interval_event_time(
				*current_interval,
				m_value.animation_interval_time,
				0.f,
				event_type,
				m_value.channel_ids,
				m_value.domain_data,
				start_time_may_be_used
			) * driving_animation_factor;
			u32 const new_event_time_in_ms = get_time_in_ms(
				m_value.event_time_in_ms,
				animation_state_interval_time*driving_animation_factor,
				new_event_time,
				event_type
			);
			if ( initial_event_types && (new_event_time_in_ms != m_value.event_time_in_ms) )
				return;

			m_value.event_type |= event_type, new_event_time /= driving_animation_factor, m_value.event_time_in_ms = new_event_time_in_ms, m_value.animation_interval_time = new_event_time;

			if ( m_value.event_type & time_event_animation_interval_ended ) {
				if ( !m_value.animation_interval_id || (--m_value.animation_interval_id < m_animation->start_cycle_animation_interval_id()) )
					m_value.animation_interval_id = m_animation->animation_intervals_count() - 1;
				m_value.animation_interval_time = m_animation->animation_intervals()[m_value.animation_interval_id].length();
			}
		}

		if ( initial_event_types || (m_value.event_time_in_ms != value.event_time_in_ms) )
			break;
	}

	if ( (m_animation->playback_type() == play_once_and_remove_at_end) &&
		 (m_value.event_type & time_event_animation_ended_in_positive_direction) )
		m_value.event_type				|= time_event_animation_lexeme_ended;
}

n_ary_tree_animation_event_iterator& n_ary_tree_animation_event_iterator::operator ++	( )
{
	advance								( 0 );
	return								*this;
}
