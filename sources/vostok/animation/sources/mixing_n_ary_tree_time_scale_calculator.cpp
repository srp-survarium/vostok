////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include "mixing_n_ary_tree_destroyer.h"
#include "mixing_animation_state.h"

using vostok::animation::mixing::n_ary_tree_time_scale_calculator;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::mixing::n_ary_tree_weight_transition_node;
using vostok::animation::mixing::n_ary_tree_time_scale_transition_node;
using vostok::animation::mixing::n_ary_tree_weight_node;
using vostok::animation::mixing::n_ary_tree_time_scale_node;
using vostok::animation::mixing::n_ary_tree_addition_node;
using vostok::animation::mixing::n_ary_tree_subtraction_node;
using vostok::animation::mixing::n_ary_tree_multiplication_node;

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_animation_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_weight_transition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

void n_ary_tree_time_scale_calculator::remove_transition( n_ary_tree_time_scale_transition_node& node )
{
	if ( !m_animation )
		return;

	u32 const end_transition_time_in_ms	= node.start_time_in_ms( ) + math::floor( node.interpolator( ).transition_time( )*1000.f );
	float const animation_interval_length	= m_animation->animation_intervals( )[ m_animation->animation_state( ).animation_interval_id ].length( );
	float animation_time_when_transition_ended	=
		m_previous_animation_time +
		m_time_scale*float( end_transition_time_in_ms >= m_previous_time_in_ms ? end_transition_time_in_ms - m_previous_time_in_ms : 0 )/1000.f;
	animation_time_when_transition_ended	=
		math::min( math::max( animation_time_when_transition_ended, 0.f ), animation_interval_length );

	n_ary_tree_time_scale_node& time_scale_node	= static_cast_checked< n_ary_tree_time_scale_node& >( node.to( ) );
	time_scale_node.set_time_scale_start_time	( end_transition_time_in_ms, animation_time_when_transition_ended );

	m_result	= &node.to( );

	n_ary_tree_destroyer destroyer;
	node.from( ).accept	( destroyer );
	node.~n_ary_tree_time_scale_transition_node	( );

	if ( m_recursion_level != 1 )
		return;

	n_ary_tree_base_node** const operands	= m_animation->operands( sizeof( n_ary_tree_animation_node ) );
	if ( m_time_scale == 1.f ) {
		std::copy	(
			operands + 1,
			operands + m_animation->operands_count( ),
			operands
		);
		m_animation->decrement_operands_count	( );
		return;
	}

	*operands	= m_result;
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_time_scale_transition_node& node )
{
	++m_recursion_level;
	++m_transitions_count;

	m_interpolator		= &node.interpolator();

	float const transition_time		= float( m_current_time_in_ms - node.start_time_in_ms() )/1000.f;
	float const interpolated_value	= (transition_time >= node.interpolator().transition_time()) ? 1.f : node.interpolator().interpolated_value( transition_time );
	if ( interpolated_value == 1.f ) {
		node.to().accept( *this );
		remove_transition	( node );
	}
	else {
		bool from_changed	= false;
		node.from().accept	( *this );
		if ( m_result ) {
			node.on_from_changed	( *m_result );
			from_changed		= true;
		}

		float const time_scale_from	= m_time_scale;
		node.to().accept	( *this );
		float const time_scale_to	= m_time_scale;
		if ( from_changed && time_scale_from == time_scale_to )
			remove_transition	( node );
		else
			m_time_scale	= time_scale_from*(1.f - interpolated_value) + time_scale_to*interpolated_value;
	}

	--m_recursion_level;
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_weight_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_time_scale_node& node )
{
	m_result			= 0;
	m_time_scale		= node.time_scale( );
	m_interpolator		= &node.interpolator();
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_addition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_subtraction_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_multiplication_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

n_ary_tree_time_scale_calculator::n_ary_tree_time_scale_calculator		(
		u32 const current_time_in_ms,
		float const previous_animation_time,
		u32 const previous_time_in_ms,
		n_ary_tree_animation_node* const animation
	) :
	m_animation					( animation ),
	m_result					( 0 ),
	m_interpolator				( 0 ),
	m_current_time_in_ms		( current_time_in_ms ),
	m_previous_animation_time	( previous_animation_time ),
	m_previous_time_in_ms		( previous_time_in_ms ),
	m_time_scale				( 0.f ),
	m_transitions_count			( 0 ),
	m_recursion_level			( 0 )
{
}
