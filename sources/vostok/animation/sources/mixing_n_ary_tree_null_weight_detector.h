// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_MIXING_N_ARY_TREE_NULL_WEIGHT_DETECTOR_H_INCLUDED
#define ANIMATION_MIXING_N_ARY_TREE_NULL_WEIGHT_DETECTOR_H_INCLUDED

#include "mixing_n_ary_tree_visitor.h"

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_null_weight_detector : public n_ary_tree_visitor {
public:
	inline	explicit	n_ary_tree_null_weight_detector	( n_ary_tree_animation_node& animation ) { /* no source */ }
	inline	bool		result							( ) const { return m_result; }

	virtual	void		visit							( n_ary_tree_animation_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_weight_transition_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_time_scale_transition_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_weight_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_time_scale_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_addition_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_subtraction_node& node ) override { /* no source */ }
	virtual	void		visit							( n_ary_tree_multiplication_node& node ) override { /* no source */ }

private:
	/* 0x0000 */	/* n_ary_tree_visitor */
	/* 0x0004 */	bool	m_result;
}; // class n_ary_tree_null_weight_detector

STATIC_SIZE_ASSERT(n_ary_tree_null_weight_detector, 0x8);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_MIXING_N_ARY_TREE_NULL_WEIGHT_DETECTOR_H_INCLUDED
