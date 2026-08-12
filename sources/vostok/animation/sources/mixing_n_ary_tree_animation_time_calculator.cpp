////////////////////////////////////////////////////////////////////////////
//	Created		: 29.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_animation_time_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_animation_state.h"

using vostok::animation::mixing::n_ary_tree_animation_time_calculator;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::mixing::n_ary_tree_weight_transition_node;
using vostok::animation::mixing::n_ary_tree_time_scale_transition_node;
using vostok::animation::mixing::n_ary_tree_weight_node;
using vostok::animation::mixing::n_ary_tree_time_scale_node;
using vostok::animation::mixing::n_ary_tree_addition_node;
using vostok::animation::mixing::n_ary_tree_subtraction_node;
using vostok::animation::mixing::n_ary_tree_multiplication_node;

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_animation_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER				( node );
	NODEFAULT								( );
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_weight_transition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER				( node );
	NODEFAULT								( );
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_weight_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER				( node );
	NODEFAULT								( );
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_addition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER				( node );
	NODEFAULT								( );
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_subtraction_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER				( node );
	NODEFAULT								( );
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_multiplication_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER				( node );
	NODEFAULT								( );
}

float n_ary_tree_animation_time_calculator::computed_animation_time	(
		const float		animation_time_before_scale_starts,
		const u32		time_scale_start_time_in_ms,
		const u32		current_time_in_ms,
		const u32		target_time_in_ms,
		const float		time_scale
	)
{
	if ( !m_animation.time_calculator() )
		return								animation_time_before_scale_starts + (current_time_in_ms - time_scale_start_time_in_ms)*time_scale/1000.f;

	return									m_animation.time_calculator()(
												animation_time_before_scale_starts,
												m_animation.animation_intervals()->length(),
												time_scale_start_time_in_ms,
												current_time_in_ms,
												target_time_in_ms,
												time_scale
											);
}

void n_ary_tree_animation_time_calculator::fill_time	(
		float const	time_scale,
		float const	animation_time_before_scale_starts,
		u32 const		time_scale_start_time_in_ms
	)
{
	m_animation_time						= computed_animation_time(
												animation_time_before_scale_starts,
												time_scale_start_time_in_ms,
												m_start_time_in_ms,
												m_target_time_in_ms,
												time_scale
											);
	m_animation_time						=
		math::min(
			math::max( m_animation_time, 0.f ),
			m_animation_interval_length
		);
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_time_scale_node& node )
{
	R_ASSERT_CMP							( m_target_time_in_ms, >=, node.time_scale_start_time_in_ms() );
	m_animation_time						= computed_animation_time(
		node.animation_time_before_scale_starts( ),
		node.time_scale_start_time_in_ms( ),
		m_target_time_in_ms,
		m_start_time_in_ms,
		node.time_scale( )
	);
	m_animation_time						= math::min( math::max( m_animation_time, 0.f ), m_animation_interval_length);
	R_ASSERT_CMP							( m_animation_time, >=, 0.f );
	R_ASSERT_CMP							( m_animation_time, <=, m_animation_interval_length );
}

void n_ary_tree_animation_time_calculator::visit		( n_ary_tree_time_scale_transition_node& node )
{
	m_animation_time						= m_start_animation_time;
	R_ASSERT_CMP							( m_target_time_in_ms, >=, m_start_time_in_ms );

	n_ary_tree_base_node* time_scale_node	= &node;

	u32 const full_intervals_count			= (m_target_time_in_ms - m_start_time_in_ms)/integration_interval_length_in_ms;
	for ( u32 i = 0; i <= full_intervals_count; ++i ) {
		u32 const start_interval_time_in_ms		= i < full_intervals_count ? (m_start_time_in_ms + (2*i+1)*(integration_interval_length_in_ms/2)) : m_target_time_in_ms;
		n_ary_tree_time_scale_calculator time_scale_calculator(
			start_interval_time_in_ms,
			m_animation_time,
			i < full_intervals_count ? (m_start_time_in_ms + (i ? 2*i-1 : 0)*(integration_interval_length_in_ms/2)) : (m_start_time_in_ms + full_intervals_count*integration_interval_length_in_ms),
			m_is_read_only ? 0 : &m_animation
		);
		if ( time_scale_node ) {
			u32 const operands_count		= m_animation.operands_count( );
			time_scale_node->accept			( time_scale_calculator );
			time_scale_node					= time_scale_calculator.result( ) ? time_scale_calculator.result( ) : time_scale_node;
			if ( operands_count != m_animation.operands_count( ) )
				time_scale_node				= 0;
		}

		m_animation_time					= computed_animation_time(
			m_animation_time,
			m_start_time_in_ms + i*integration_interval_length_in_ms,
			i < full_intervals_count ? m_start_time_in_ms + (i+1)*integration_interval_length_in_ms : m_target_time_in_ms,
			m_start_time_in_ms + i*integration_interval_length_in_ms,
			time_scale_node ? time_scale_calculator.time_scale( ) : 1.f
		);
		m_animation_time					= math::min( math::max( m_animation_time, 0.f ), m_animation_interval_length);
		R_ASSERT_CMP						( m_animation_time, >=, 0.f );
		R_ASSERT_CMP						( m_animation_time, <=, m_animation_interval_length );
	}
}

n_ary_tree_animation_time_calculator::n_ary_tree_animation_time_calculator	(
		n_ary_tree_animation_node& animation,
		u32 const start_time_in_ms,
		float const start_animation_time,
		u32 const target_time_in_ms,
		bool const is_read_only
	) :
	m_animation								( animation ),
	m_start_time_in_ms						( start_time_in_ms ),
	m_start_animation_time					( start_animation_time ),
	m_target_time_in_ms						( target_time_in_ms ),
	m_animation_interval_length				( (animation.animation_intervals() + animation.animation_state().animation_interval_id)->length() ),
	m_is_read_only							( false )
{
	if ( animation.operands_count( ) && (*animation.operands( sizeof( n_ary_tree_animation_node ) ))->is_time_scale( ) )
		(*animation.operands( sizeof( n_ary_tree_animation_node ) ))->accept	( *this );
	else
		fill_time							( 1.f, start_animation_time, start_time_in_ms );

	R_ASSERT_CMP							( m_animation_time, >=, 0.f );
	R_ASSERT_CMP							( m_animation_time, <=, m_animation_interval_length );
}
