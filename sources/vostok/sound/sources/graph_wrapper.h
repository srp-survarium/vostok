////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SOURCES_GRAPH_WRAPPER_H_INCLUDED
#define VOSTOK_SOUND_SOURCES_GRAPH_WRAPPER_H_INCLUDED

#include <vostok/sound/search/graph_wrapper.h>
#include <vostok/sound/search/search_service.h>

namespace vostok {
namespace sound {
namespace search {

template < typename vertex_type >
inline void graph_wrapper::edge_iterators(
	vertex_type const& vertex,
	const_edge_iterator& begin,
	const_edge_iterator& end
)
{
	render::culling::portal const& current_portal = m_graph->get_portals( )[vertex.id( ).portal_id];
	render::culling::spatial_sector const& current_sector = m_graph->get_sectors( )[
		vertex.id( ).incoming_sector_index ? current_portal.get_sectors( )[0] : current_portal.get_sectors( )[1]
	];
	begin = current_sector.get_portals( );
	end = current_sector.get_portals( ) + current_sector.get_portals_count( );
}

inline vertex_id_type graph_wrapper::vertex_id(
	vertex_id_type const& vertex_id,
	const_edge_iterator const iterator
) const
{
	render::culling::portal const& current_portal = m_graph->get_portals( )[vertex_id.portal_id];
	u32 const current_sector_id = vertex_id.incoming_sector_index ? current_portal.get_sectors( )[0] : current_portal.get_sectors( )[1];
	render::culling::portal const& next_portal = m_graph->get_portals( )[*iterator];

	vertex_id_type result;
	result.portal_id = *iterator;
	result.incoming_sector_index = next_portal.get_sectors( )[0] != current_sector_id;
	return result;
}

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SOURCES_GRAPH_WRAPPER_H_INCLUDED
