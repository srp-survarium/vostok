////////////////////////////////////////////////////////////////////////////
//	Created		: 03.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_binary_tree_initializer.h"

#include <vostok/animation/mixing_binary_tree_addition_node.h>
#include <vostok/animation/mixing_binary_tree_animation_node.h>
#include <vostok/animation/mixing_binary_tree_multiplication_node.h>
#include <vostok/animation/mixing_binary_tree_subtraction_node.h>
#include <vostok/animation/mixing_binary_tree_weight_node.h>

namespace vostok {
namespace animation {
namespace mixing {

binary_tree_initializer::binary_tree_initializer( binary_tree_base_node& expression_node ) :
	m_root( expression_node )
{
	expression_node.accept( *this );
}

void binary_tree_initializer::visit( binary_tree_animation_node& node )
{
	node.m_next_weight_animation		= 0;
	node.m_n_ary_animation			= 0;
	node.m_unique_weights_count		= 0;
	node.m_null_weight_found		= false;
	node.m_next_weight				= 0;
	node.m_same_weight				= 0;
	node.m_next_unique_interpolator	= 0;

	if ( node.weight_driving_animation() )
		node.m_weight_interpolator	= 0;
}

void binary_tree_initializer::visit( binary_tree_weight_node& node )
{
	node.m_next_weight				= 0;
	node.m_same_weight				= 0;
	node.m_next_unique_interpolator	= 0;
}

void binary_tree_initializer::visit( binary_tree_addition_node& node )
{
	node.left().accept( *this );
	node.right().accept( *this );

	node.m_next_weight				= 0;
	node.m_same_weight				= 0;
	node.m_next_unique_interpolator	= 0;
}

void binary_tree_initializer::visit( binary_tree_subtraction_node& node )
{
	node.left().accept( *this );
	node.right().accept( *this );

	node.m_next_weight				= 0;
	node.m_same_weight				= 0;
	node.m_next_unique_interpolator	= 0;
}

void binary_tree_initializer::visit( binary_tree_multiplication_node& node )
{
	node.left().accept( *this );
	node.right().accept( *this );

	node.m_next_weight				= 0;
	node.m_same_weight				= 0;
	node.m_next_unique_interpolator	= 0;
}

} // namespace mixing
} // namespace animation
} // namespace vostok
