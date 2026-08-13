////////////////////////////////////////////////////////////////////////////
//	Created		: 26.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_weight_event_iterator.h"
#include "mixing_n_ary_tree_weight_transition_end_time_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include "mixing_animation_state.h"

namespace vostok {
namespace animation {
namespace mixing {

void n_ary_tree_weight_transition_end_time_calculator::visit( n_ary_tree_animation_node& node )
{
	// claude@NOTE: target spills min_weight before the virtual call and reloads float_max; base hoists float_max into xmm2.
	u32 const operands_count		= node.operands_count( );
	n_ary_tree_base_node** i		= node.operands( sizeof(n_ary_tree_animation_node) );
	n_ary_tree_base_node** const e	= i + operands_count;

	if ( operands_count && (*i)->is_time_scale() )
		++i;

	m_min_weight					= math::float_max;
	m_weight_transition_end_time_in_ms	= u32( -1 );

	for ( ; i != e; ++i ) {
		float const min_weight		= m_min_weight;
		u32 const weight_transition_end_time	= m_weight_transition_end_time_in_ms;

		m_min_weight				= math::float_max;
		m_weight_transition_end_time_in_ms	= u32( -1 );
		(*i)->accept				( *this );

		if ( m_min_weight > min_weight ) {
			m_min_weight			= min_weight;
			if ( min_weight == math::float_max )
				m_weight_transition_end_time_in_ms	= weight_transition_end_time;
			else
				m_weight_transition_end_time_in_ms	= math::min( m_weight_transition_end_time_in_ms, weight_transition_end_time );
		}
		else if ( m_min_weight == min_weight ) {
			if ( min_weight == math::float_max )
				m_weight_transition_end_time_in_ms	= weight_transition_end_time;
			else
				m_weight_transition_end_time_in_ms	= math::min( m_weight_transition_end_time_in_ms, weight_transition_end_time );
		}
	}

	if ( m_weight_transition_end_time_in_ms == u32( -1 ) ) {
		m_event_type				= 0;
		return;
	}

	m_event_type					= time_event_weight_transitions_ended;
	if ( m_min_weight == 0.f )
		m_event_type				|= time_event_animation_lexeme_ended;
}

void n_ary_tree_weight_transition_end_time_calculator::visit( n_ary_tree_weight_transition_node& node )
{
	node.from( ).accept			( *this );
	u32 const weight_transition_end_time	=
		node.start_time_in_ms( ) + math::floor( node.interpolator( ).transition_time( ) * 1000.f );
	float const min_weight			= node.to( ).is_transition( )
		? m_min_weight
		: static_cast_checked< n_ary_tree_weight_node& >( node.to( ) ).weight( );

	if ( m_min_weight == min_weight )
		m_weight_transition_end_time_in_ms	= math::min( weight_transition_end_time, m_weight_transition_end_time_in_ms );
	else {
		m_weight_transition_end_time_in_ms	= weight_transition_end_time;	 m_min_weight = min_weight;
	}
}

void n_ary_tree_weight_transition_end_time_calculator::visit( n_ary_tree_time_scale_transition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER		( node );
	NODEFAULT						( );
}

void n_ary_tree_weight_transition_end_time_calculator::visit( n_ary_tree_weight_node& node )
{
	m_min_weight					= node.weight( );
}

void n_ary_tree_weight_transition_end_time_calculator::visit( n_ary_tree_addition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER		( node );
}

n_ary_tree_weight_event_iterator::n_ary_tree_weight_event_iterator(
		n_ary_tree_animation_node& animation,
		u32 const start_time_in_ms,
		u32 const initial_event_types
	) :
	m_animation											( &animation )
{
	n_ary_tree_weight_transition_end_time_calculator	weight_transition_end_time_calculator;
	weight_transition_end_time_calculator.visit			( animation );
	m_time_in_ms										= weight_transition_end_time_calculator.weight_transition_end_time( );
	if ( m_time_in_ms == u32(-1) ) {
		m_animation										= 0;
		m_event_type									= 0;
		return;
	}

	if ( !initial_event_types ) {
		m_event_type									= weight_transition_end_time_calculator.event_type();
		return;
	}

	m_time_in_ms										= start_time_in_ms;
	m_event_type										= time_event_weight_transitions_started;
}

n_ary_tree_weight_event_iterator& n_ary_tree_weight_event_iterator::operator ++	( )
{
	n_ary_tree_weight_transition_end_time_calculator	weight_transition_end_time_calculator;
	weight_transition_end_time_calculator.visit			( *m_animation );
	m_time_in_ms										= weight_transition_end_time_calculator.weight_transition_end_time( );
	m_event_type										= weight_transition_end_time_calculator.event_type();
	if ( m_time_in_ms == u32(-1) ) {
		m_animation										= 0;
		m_event_type									= 0;
	}

	return												*this;
}

void n_ary_tree_weight_event_iterator::invert_times( u32 const time_in_ms )
{
	if ( m_event_type )
		m_time_in_ms			= time_in_ms - m_time_in_ms;
}

} // namespace mixing
} // namespace animation
} // namespace vostok
