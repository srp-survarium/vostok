////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animations_search_service.h"
#include "animation_space_heuristics.h"
#include "animation_space_search_restrictor.h"

namespace survarium {

// STATE[STUB]
// claude@NOTE: 26-statement A* search (4 named locals: graph_wrapper, path_heuristics,
// search_restrictor, path_constructor). PARKED `return false` buildability stub; the
// real body constructs the A* machinery (ai::a_star over the animation-space graph) and
// runs the search. Needs the search_base template idiom reconstructed. Structure
// @0x7b62a0 (--view target).
bool animations_search_service::search(
	animation_space_graph_ptr const&	graph,
	vector< u32 >*						path,
	const animation_space_vertex_id		arg_2 /* animation_space_vertex_id start_vertex_id */,
	const animation_space_vertex_id		arg_3 /* animation_space_vertex_id target_vertex_id */
)
{
	return false;
}

} // namespace survarium
