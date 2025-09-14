////////////////////////////////////////////////////////////////////////////
//	Created		: 29.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_interpolator_selector.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"

using vostok::animation::mixing::n_ary_tree_interpolator_selector;
using vostok::animation::mixing::n_ary_tree_animation_node;
using vostok::animation::mixing::n_ary_tree_weight_transition_node;
using vostok::animation::mixing::n_ary_tree_time_scale_transition_node;
using vostok::animation::mixing::n_ary_tree_weight_node;
using vostok::animation::mixing::n_ary_tree_time_scale_node;
using vostok::animation::mixing::n_ary_tree_addition_node;
using vostok::animation::mixing::n_ary_tree_multiplication_node;
using vostok::animation::mixing::n_ary_tree_subtraction_node;
using vostok::animation::base_interpolator;

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_animation_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	UNREACHABLE_CODE			( );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_weight_transition_node& node )
{
	node.to().accept			( *this );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_time_scale_transition_node& node )
{
	node.to().accept			( *this );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_weight_node& node )
{
	m_result					= &node.interpolator( );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_time_scale_node& node )
{
	m_result					= &node.interpolator( );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_addition_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	UNREACHABLE_CODE			( );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_subtraction_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	UNREACHABLE_CODE			( );
}

void n_ary_tree_interpolator_selector::visit				( n_ary_tree_multiplication_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	UNREACHABLE_CODE			( );
}