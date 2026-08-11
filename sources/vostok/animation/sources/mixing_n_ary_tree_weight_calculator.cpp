////////////////////////////////////////////////////////////////////////////
//	Unit		: n_ary_tree_weight_calculator.cpp
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_weight_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include "mixing_n_ary_tree_destroyer.h"

using vostok::animation::mixing::n_ary_tree_weight_calculator;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::mixing::n_ary_tree_weight_transition_node;
using vostok::animation::mixing::n_ary_tree_time_scale_transition_node;
using vostok::animation::mixing::n_ary_tree_weight_node;
using vostok::animation::mixing::n_ary_tree_time_scale_node;
using vostok::animation::mixing::n_ary_tree_addition_node;
using vostok::animation::mixing::n_ary_tree_subtraction_node;
using vostok::animation::mixing::n_ary_tree_multiplication_node;

void n_ary_tree_weight_calculator::visit		( n_ary_tree_animation_node& node )
{
	float weight		= 1.f;

	n_ary_tree_base_node** i		= node.operands( sizeof(n_ary_tree_animation_node) );
	u32 const operands_count		= node.operands_count( );
	n_ary_tree_base_node** e		= i + operands_count;

	if ( operands_count && (*i)->is_time_scale() ) {
		++i;
	}

	for ( ; i != e; ++i ) {
		R_ASSERT		( (*i)->is_weight() );
		R_ASSERT		( !(*i)->is_time_scale() );
		(*i)->accept	( *this );
		weight			*= m_weight;
		if ( m_result ) {
			if ( node.operands_count() == operands_count )
				*i			= m_result;
			else {
				--i;
				--e;
			}
		}

		if ( weight == 0.f )
			break;
	}

	m_weight			= weight;
	m_result			= 0;
}

void n_ary_tree_weight_calculator::remove_transition( n_ary_tree_weight_transition_node& node )
{
	if ( m_weight == 0.f ) {
		m_weight_transition_ended_time_in_ms	= node.start_time_in_ms() + math::floor( node.interpolator().transition_time()*1000.f );
		m_null_weight_found	= true;
	}

	if ( !m_animation ) {
		m_result			= 0;
		return;
	}

	m_result				= &node.to();
	n_ary_tree_destroyer destroyer;
	node.from().accept	( destroyer );
	node.~n_ary_tree_weight_transition_node	( );

	if ( m_recursion_level != 1 )
		return;

	n_ary_tree_base_node** const operands	= m_animation->operands( sizeof(n_ary_tree_animation_node) );
	if ( m_weight == 1.f ) {
		n_ary_tree_base_node** i		= operands;
		n_ary_tree_base_node** j		= i;
		n_ary_tree_base_node** const e	= i + m_animation->operands_count();
		for ( ; j != e; ++j ) {
			if ( *i == &node )
				continue;

			*i++			= *j;
		}

		m_animation->decrement_operands_count();
		return;
	}

	n_ary_tree_base_node** i		= operands;
	n_ary_tree_base_node** const e	= i + m_animation->operands_count();
	for ( ; i != e; ++i ) {
		if ( *i == &node )
			*i				= m_result;
	}
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_weight_transition_node& node )
{
	++m_recursion_level;

	float const transition_time		= float( m_current_time_in_ms - node.start_time_in_ms() )/1000.f;
	float const interpolated_value	= (transition_time >= node.interpolator().transition_time()) ? 1.f : node.interpolator().interpolated_value( transition_time );
	if ( interpolated_value == 1.f ) {
		node.to().accept	( *this );
		remove_transition	( node );
	}
	else {
		bool from_changed	= false;
		node.from().accept	( *this );
		if ( m_result ) {
			node.on_from_changed	( *m_result );
			from_changed		= true;
		}

		float const weight_from	= m_weight;
		node.to().accept	( *this );
		float const weight_to	= m_weight;
		if ( from_changed && (weight_from == weight_to) )
			remove_transition	( node );
		else {
			m_weight			= weight_from*(1.f - interpolated_value) + weight_to*interpolated_value;
			m_null_weight_found	= false;
		}
	}

	--m_recursion_level;
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_time_scale_transition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( m_weight = 1.f );
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_weight_node& node )
{
	m_weight			= node.weight( );
	m_result			= 0;
	m_null_weight_found	= m_weight == 0.f;
	m_weight_transition_ended_time_in_ms	= m_current_time_in_ms;
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_time_scale_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT			( m_weight = 1.f );
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_addition_node& node )
{
	float weight						= 0.f;

	u32 const operands_count			= node.operands_count( );
	R_ASSERT							( operands_count );
	n_ary_tree_base_node** i			= node.operands( sizeof(n_ary_tree_addition_node) );
	n_ary_tree_base_node** const e		= i + operands_count;
	for ( ; i != e; ++i ) {
		(*i)->accept	( *this );
		weight			+= m_weight;

		if ( !m_result )
			continue;

		*i				= m_result;
	}

	m_weight			= weight;
	m_result			= 0;
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_subtraction_node& node )
{
	bool first_operand	= true;
	float weight		= 0.f;

	u32 const operands_count		= node.operands_count( );
	R_ASSERT						( operands_count );
	n_ary_tree_base_node** i		= node.operands( sizeof(n_ary_tree_subtraction_node) );
	n_ary_tree_base_node** const e	= i + operands_count;
	for ( ; i != e; ++i ) {
		(*i)->accept	( *this );
		if ( first_operand ) {
			first_operand	= false;
			weight		= m_weight;
		}
		else
			weight		-= m_weight;

		if ( !m_result )
			continue;

		*i				= m_result;
	}

	m_weight			= weight;
	m_result			= 0;
}

void n_ary_tree_weight_calculator::visit		( n_ary_tree_multiplication_node& node )
{
	float weight		= 1.f;

	u32 const operands_count		= node.operands_count( );
	R_ASSERT						( operands_count );
	n_ary_tree_base_node** i		= node.operands( sizeof(n_ary_tree_multiplication_node) );
	n_ary_tree_base_node** const e	= i + operands_count;
	for ( ; i != e; ++i ) {
		(*i)->accept	( *this );
		weight			*= m_weight;
		if ( weight == 0.f )
			break;

		if ( !m_result )
			continue;

		*i				= m_result;
	}

	m_weight			= weight;
	m_result			= 0;
}
