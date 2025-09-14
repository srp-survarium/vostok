////////////////////////////////////////////////////////////////////////////
//	Created		: 25.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SUBTRACTION_NODE_INLINE_H_INCLUDED
#define SUBTRACTION_NODE_INLINE_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

inline binary_tree_subtraction_node::binary_tree_subtraction_node		(
		binary_tree_base_node* const left,
		binary_tree_base_node* const right
	) :
	super	( left, right )
{
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef SUBTRACTION_NODE_INLINE_H_INCLUDED