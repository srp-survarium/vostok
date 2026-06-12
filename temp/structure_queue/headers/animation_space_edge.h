////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_EDGE_H_INCLUDED
#define ANIMATION_SPACE_EDGE_H_INCLUDED

/* INCLUDES */
struct stlp_std::pair<survarium::animation_space_vertex const *,survarium::animation_space_vertex const *>;
struct survarium::animation_space_vertex_id;

namespace survarium {

struct animation_space_edge {
	inline		animation_space_edge(
					animation_space_vertex_id const&	arg_0,
					std::pair< animation_space_vertex const*, animation_space_vertex const* > const*	arg_1,
					const float							arg_2,
					const float							arg_3
				) { /* no source */ }

public:
	/* 0x0000 */	animation_space_vertex_id		movement;
	/* 0x001c */	std::pair< animation_space_vertex const*, animation_space_vertex const* > const*	mixable_pair;
	/* 0x0020 */	float							first_animation_weight;
	/* 0x0024 */	float							animation_length;
}; // struct animation_space_edge

STATIC_SIZE_ASSERT(animation_space_edge, 0x28);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_EDGE_H_INCLUDED
