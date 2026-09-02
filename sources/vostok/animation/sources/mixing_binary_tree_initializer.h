// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_MIXING_BINARY_TREE_INITIALIZER_H_INCLUDED
#define ANIMATION_MIXING_BINARY_TREE_INITIALIZER_H_INCLUDED

#include <vostok/animation/mixing_binary_tree_visitor.h>

namespace vostok {
namespace animation {
namespace mixing {

struct binary_tree_base_node;

class binary_tree_initializer : public binary_tree_visitor , private boost::noncopyable {
public:
			explicit					binary_tree_initializer	( binary_tree_base_node& expression_node );

	inline	binary_tree_base_node&		result					( ) const { return m_root; }

private:
	virtual	void						visit					( binary_tree_animation_node& node ) override;
	virtual	void						visit					( binary_tree_weight_node& node ) override;
	virtual	void						visit					( binary_tree_addition_node& node ) override;
	virtual	void						visit					( binary_tree_subtraction_node& node ) override;
	virtual	void						visit					( binary_tree_multiplication_node& node ) override;

private:
	/* 0x0000 */	/* binary_tree_visitor */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	binary_tree_base_node&		m_root;
}; // class binary_tree_initializer

STATIC_SIZE_ASSERT(binary_tree_initializer, 0x8);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_MIXING_BINARY_TREE_INITIALIZER_H_INCLUDED
