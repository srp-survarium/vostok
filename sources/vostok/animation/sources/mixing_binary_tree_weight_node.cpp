////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/mixing_binary_tree_weight_node.h>
#include <vostok/animation/base_interpolator.h>
#include <vostok/animation/mixing_binary_tree_visitor.h>

using vostok::animation::mixing::binary_tree_weight_node;
using vostok::animation::mixing::binary_tree_visitor;
using vostok::animation::mixing::binary_tree_base_node;

void binary_tree_weight_node::accept	( binary_tree_visitor& visitor )
{
	visitor.visit	( *this );
}