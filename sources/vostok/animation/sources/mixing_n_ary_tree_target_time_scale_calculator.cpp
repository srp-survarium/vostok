////////////////////////////////////////////////////////////////////////////
//	Created		: 05.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_target_time_scale_calculator.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include "mixing_n_ary_tree_destroyer.h"

using vostok::animation::mixing::n_ary_tree_target_time_scale_calculator;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::mixing::n_ary_tree_weight_transition_node;
using vostok::animation::mixing::n_ary_tree_time_scale_transition_node;
using vostok::animation::mixing::n_ary_tree_weight_node;
using vostok::animation::mixing::n_ary_tree_time_scale_node;
using vostok::animation::mixing::n_ary_tree_addition_node;
using vostok::animation::mixing::n_ary_tree_subtraction_node;
using vostok::animation::mixing::n_ary_tree_multiplication_node;

n_ary_tree_target_time_scale_calculator::n_ary_tree_target_time_scale_calculator( n_ary_tree_animation_node& node )
{
	(*node.operands( sizeof(n_ary_tree_animation_node) ))->accept	( *this );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_animation_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT					( );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_weight_transition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT					( );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_time_scale_transition_node& node )
{
	node.to().accept( *this );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_weight_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT					( );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_time_scale_node& node )
{
	m_result	= node.time_scale( );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_addition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT					( );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_subtraction_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT					( );
}

void n_ary_tree_target_time_scale_calculator::visit		( n_ary_tree_multiplication_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT					( );
}