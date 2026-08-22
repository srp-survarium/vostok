////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animations_search_service.h"
#include "animation_space_heuristics.h"
#include "animation_space_search_restrictor.h"

namespace survarium {

bool animations_search_service::search(
	animation_space_graph_ptr const&	graph,
	vector< u32 >*						path,
	const animation_space_vertex_id		arg_2 /* animation_space_vertex_id start_vertex_id */,
	const animation_space_vertex_id		arg_3 /* animation_space_vertex_id target_vertex_id */
)
{
	typedef ai::path_constructor::edge::impl< vertex_type, vector< u32 > > path_constructor_type;

	animation_space_graph_wrapper graph_wrapper( graph, arg_2 );
	path_constructor_type path_constructor( path );
	animation_space_heuristics path_heuristics( graph, arg_3, graph->max_speed( ) );
	animation_space_search_restrictor search_restrictor(
		graph,
		arg_2,
		arg_3,
		std::numeric_limits< float >::max( ),
		0x400 - graph->get_edges_count( ),
		u32( -1 )
	);
	if (
		ai::a_star::find(
			m_priority_queue,
			graph_wrapper,
			path_constructor,
			path_heuristics,
			search_restrictor
		)
	)
		return true;

	path_constructor.construct_path(
		m_vertex_manager.vertex(
			m_vertex_manager.visited( path_heuristics.best_vertex_id( ) ) ? path_heuristics.best_vertex_id( ) : arg_2
		)
	);

	return true;
}

} // namespace survarium
