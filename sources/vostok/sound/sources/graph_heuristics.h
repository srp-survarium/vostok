////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SOURCES_GRAPH_HEURISTICS_H_INCLUDED
#define VOSTOK_SOUND_SOURCES_GRAPH_HEURISTICS_H_INCLUDED

#include <vostok/sound/search/graph_heuristics.h>

namespace vostok {
namespace sound {
namespace search {

inline float graph_heuristics::estimate(
	vertex_id_type const* const current_vertex_id_ptr,
	vertex_id_type const& neighbour_vertex_id
) const
{
	return ( m_target_position - m_graph->get_portals( )[neighbour_vertex_id.portal_id].get_points( )[0] ).length( );
}

template < typename vertex_type >
inline float graph_heuristics::evaluate(
	vertex_type const& current_vertex,
	vertex_type const& neighbour_vertex,
	u32 const* const&
) const
{
	return (
		m_graph->get_portals( )[neighbour_vertex.id( ).portal_id].get_points( )[0] -
		m_graph->get_portals( )[current_vertex.id( ).portal_id].get_points( )[0]
	).length( ) + current_vertex.g( ) + current_vertex.id( ).source_to_portal_distance;
}

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SOURCES_GRAPH_HEURISTICS_H_INCLUDED
