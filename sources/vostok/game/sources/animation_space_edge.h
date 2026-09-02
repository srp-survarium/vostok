// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_SPACE_EDGE_H_INCLUDED
#define ANIMATION_SPACE_EDGE_H_INCLUDED

#include "animation_space_vertex_id.h"

namespace survarium {

struct animation_space_vertex;

struct animation_space_edge {
public:
	/* 0x0000 */	animation_space_vertex_id		movement;
	/* 0x001c */	std::pair< animation_space_vertex const*, animation_space_vertex const* > const*	mixable_pair;
	/* 0x0020 */	float							first_animation_weight;
	/* 0x0024 */	float							animation_length;

	inline		animation_space_edge(
					animation_space_vertex_id const&	movement,
					std::pair< animation_space_vertex const*, animation_space_vertex const* > const*	mixable_pair,
					const float							first_animation_weight,
					const float							animation_length
				) :
					movement				( movement ),
					mixable_pair			( mixable_pair ),
					first_animation_weight	( first_animation_weight ),
					animation_length		( animation_length )
				{}
}; // struct animation_space_edge

STATIC_SIZE_ASSERT(animation_space_edge, 0x28);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_EDGE_H_INCLUDED
