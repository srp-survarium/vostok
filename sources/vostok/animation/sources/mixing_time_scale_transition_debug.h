////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_MIXING_TIME_SCALE_TRANSITION_DEBUG_H_INCLUDED
#define ANIMATION_MIXING_TIME_SCALE_TRANSITION_DEBUG_H_INCLUDED

#include "mixing_n_ary_tree_visitor.h"

namespace vostok {
namespace animation {
namespace mixing {

class time_scale_transition_debug : public n_ary_tree_visitor {
public:
	inline	explicit	time_scale_transition_debug	( n_ary_tree_time_scale_transition_node& node ) { /* no source */ }

	virtual	void		visit						( n_ary_tree_animation_node& node ) override { /* no source */ }
	virtual	void		visit						( n_ary_tree_weight_transition_node& node ) override { /* no source */ }
	virtual	void		visit						( n_ary_tree_time_scale_transition_node& node ) override;
	virtual	void		visit						( n_ary_tree_weight_node& node ) override { /* no source */ }
	virtual	void		visit						( n_ary_tree_time_scale_node& node ) override { /* no source */ }
	virtual	void		visit						( n_ary_tree_addition_node& node ) override { /* no source */ }
	virtual	void		visit						( n_ary_tree_subtraction_node& node ) override { /* no source */ }
	virtual	void		visit						( n_ary_tree_multiplication_node& node ) override { /* no source */ }
}; // class time_scale_transition_debug

STATIC_SIZE_ASSERT(time_scale_transition_debug, 0x4);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_MIXING_TIME_SCALE_TRANSITION_DEBUG_H_INCLUDED
