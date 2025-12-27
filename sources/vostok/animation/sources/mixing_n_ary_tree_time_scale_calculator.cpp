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

// STATE[STUB]
void n_ary_tree_time_scale_calculator::remove_transition( n_ary_tree_time_scale_transition_node& node )
{
	// LOCALS
	// float 							animation_interval_length
	// n_ary_tree_destroyer 			destroyer
	// float 							animation_time_when_transition_ended
	// ******

	// CALL SITE INFO
	// <0x56d430> -> float < unknown >() const
	// <0x56d4dd> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56d4e7> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY
	// <0x56d416>|0x006|+0x00d:'44'
	// <0>
	// <1>
	// <0x56d423>|0x013|+0x020:'47'
	// <0>
	// <1>
	// <2>
	// <0x56d443>|0x033|+0x020:'51'
	// <0>
	// <1>
	// <0x56d463>|0x053|+0x034:'54'
	// <0x56d497>|0x087|+0x023:'55'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x56d4ba>|0x0aa|+0x008:'63'
	// <0>
	// <0x56d4c2>|0x0b2|+0x006:'65'
	// <0>
	// <1>
	// <0x56d4c8>|0x0b8|+0x017:'68'
	// <0x56d4df>|0x0cf|+0x00a:'69'
	// <0>
	// <0x56d4e9>|0x0d9|+0x008:'71'
	// <0x56d4f1>|0x0e1|+0x012:'72'
	// <0>
	// <0x56d503>|0x0f3|+0x013:'74'
	// <0x56d516>|0x106|+0x00e:'75'
	// <0>
	// <0x56d524>|0x114|+0x00c:'77'
	// <0>
	// <1>
	// <0x56d530>|0x120|-0x004:'80'
	// <0>
	// <1>
	// <0x56d52c>|0x11c|+0x00f:'83'
	// ******
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_time_scale_transition_node& node )
{
	/* sushi@TODO
	m_interpolator		= &node.interpolator();

	R_ASSERT_CMP		( m_current_time_in_ms, >=, node.start_time_in_ms() );
	float const transition_time		= float( m_current_time_in_ms - node.start_time_in_ms() )/1000.f;
	float const interpolated_value	= (transition_time >= node.interpolator().transition_time()) ? 1.f : node.interpolator().interpolated_value( transition_time );
	R_ASSERT_CMP		( interpolated_value, >=, 0.f );
	R_ASSERT_CMP		( interpolated_value, <=, 1.f );
	if ( interpolated_value == 1.f ) {
		node.to().accept( *this );
		if ( m_transitions_destroying == forbid_transitions_destroying ) {
			m_result	= &node;
			return;
		}

		u32 const end_transition_time_in_ms					= node.start_time_in_ms() + math::floor( node.interpolator().transition_time()*1000.f );
		R_ASSERT_CMP										( end_transition_time_in_ms, <=, m_current_time_in_ms );

		R_ASSERT											( m_animation );
		float const animation_interval_length				= m_animation->animation_intervals()[ m_animation->animation_state().animation_interval_id ].length();

		n_ary_tree_time_scale_node& time_scale_node			= static_cast_checked<n_ary_tree_time_scale_node&>(node.to());
		float animation_time_when_transition_ended			= m_previous_animation_time + m_time_scale*float(end_transition_time_in_ms >= m_previous_time_in_ms ? (end_transition_time_in_ms - m_previous_time_in_ms) : 0)/1000.f;
		animation_time_when_transition_ended				= math::min( math::max( animation_time_when_transition_ended, 0.f ), animation_interval_length);
		R_ASSERT_CMP										( animation_time_when_transition_ended, >=, 0.f );
		R_ASSERT_CMP										( animation_time_when_transition_ended, <=, animation_interval_length );

		#ifndef MASTER_GOLD
		LOG_DEBUG											( "ON_TIME_SCALE_TRANSITION_ENDED: [%s] setting time scale start time: %d, %.3f", m_animation->identifier(), end_transition_time_in_ms, animation_time_when_transition_ended );
		#endif // #ifndef MASTER_GOLD

		time_scale_node.set_time_scale_start_time			( end_transition_time_in_ms, animation_time_when_transition_ended );

		m_result		= &node.to();

		n_ary_tree_destroyer	destroyer;
		node.from().accept	( destroyer );
		node.~n_ary_tree_time_scale_transition_node	( );

		return;
	}

	node.from().accept	( *this );
	float const time_scale_from	= m_time_scale;
	node.m_from			= m_result;

	node.to().accept	( *this );
	float const time_scale_to	= m_time_scale;
	node.m_to			= m_result;

	m_result			= &node;
	m_time_scale		= time_scale_from*(1.f - interpolated_value) + time_scale_to*interpolated_value;
	*/
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_weight_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( );
}

void n_ary_tree_time_scale_calculator::visit	( n_ary_tree_time_scale_node& node )
{
	m_result			= &node;
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
		n_ary_tree_animation_node const* const animation
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