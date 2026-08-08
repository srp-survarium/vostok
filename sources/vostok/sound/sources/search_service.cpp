////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/sound/search/search_service.h>
#include <vostok/ai/search/path_constructor_vertex.h>
#include "graph_wrapper.h"
#include "graph_heuristics.h"
#include "search_restrictor.h"

namespace vostok {
namespace sound {
namespace search {

bool search_service::search(
	memory::base_allocator& allocator,
	render::culling::portal_sector_structure_ptr const& graph,
	vector< vertex_id_type >* path,
	float3 const& start_position,
	float3 const& target_position,
	float const& max_distance,
	vectora< fixed_vector< u32, 32 > >& result_paths
)
{
	typedef ai::path_constructor::vertex::impl< vertex_type, vector< vertex_id_type > > path_constructor_type;

	u32 source_sector_id = graph->get_sector_id( allocator, start_position );
	u32 destination_sector_id = graph->get_sector_id( allocator, target_position );
	if ( source_sector_id == destination_sector_id )
		return true;

	path_constructor_type path_constructor( path );
	graph_wrapper graph_wrapper( graph );
	search_restrictor restrictor(
		graph,
		source_sector_id,
		destination_sector_id,
		start_position,
		max_distance,
		u32(-1),
		4096 - graph->get_portals( ).size( )
	);
	graph_heuristics heuristics( graph, target_position );

	ai::a_star::find( m_priority_queue, graph_wrapper, path_constructor, heuristics, restrictor );

	result_paths.resize( restrictor.vertex_ids( ).size( ) );
	vertex_id_type const* i = restrictor.vertex_ids( ).begin( );
	vertex_id_type const* const e = restrictor.vertex_ids( ).end( );
	for ( u32 k = 0; i != e; ++i, ++k )
	{
		result_paths[k].clear( );
		path_constructor.construct_path( m_vertex_manager.vertex( *i ) );

		{
			vertex_id_type const* i = path->begin( );
			vertex_id_type const* const e = path->end( );
			for ( ; i != e; ++i )
				result_paths[k].push_back( i->portal_id );
		}
	}

	return !result_paths.empty( );
}

} // namespace search
} // namespace sound
} // namespace vostok
