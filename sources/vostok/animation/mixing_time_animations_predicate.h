// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_MIXING_TIME_ANIMATIONS_PREDICATE_H_INCLUDED
#define ANIMATION_MIXING_TIME_ANIMATIONS_PREDICATE_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

class n_ary_tree_animation_node;

struct time_animations_predicate {
	inline bool operator ( )(
		n_ary_tree_animation_node const* const left,
		n_ary_tree_animation_node const* const right
	) const;
}; // struct time_animations_predicate

STATIC_SIZE_ASSERT(time_animations_predicate, 0x1);

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_MIXING_TIME_ANIMATIONS_PREDICATE_H_INCLUDED
