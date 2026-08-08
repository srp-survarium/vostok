////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SOURCES_SEARCH_RESTRICTOR_H_INCLUDED
#define VOSTOK_SOUND_SOURCES_SEARCH_RESTRICTOR_H_INCLUDED

#include <vostok/sound/search/search_restrictor.h>

namespace vostok {
namespace sound {
namespace search {

inline vertex_id_type search_restrictor::start_vertex_id( u32 const start_vertex_id ) const
{
	vertex_id_type result;
	render::culling::portal const& p = m_graph->get_portals( )[m_graph->get_sectors( )[m_start_sector_id].get_portals( )[start_vertex_id]];
	result.portal_id = m_graph->get_sectors( )[m_start_sector_id].get_portals( )[start_vertex_id];
	result.incoming_sector_index = p.get_sectors( )[0] != m_graph->get_sectors( )[m_start_sector_id].get_portals( )[start_vertex_id];
	result.source_to_portal_distance = math::length( p.get_points( )[0] - m_source_position );
	return result;
}

inline bool search_restrictor::target_reached( vertex_id_type const& vertex_id )
{
	render::culling::portal const& portal = m_graph->get_portals( )[vertex_id.portal_id];
	u32 const outcoming_sector_id = vertex_id.incoming_sector_index ? portal.get_sectors( )[0] : portal.get_sectors( )[1];
	if ( m_target_sector_id == outcoming_sector_id )
	{
		--m_different_paths_left;
		m_vertex_ids.push_back( vertex_id );
	}

	return false;
}

inline u32 search_restrictor::get_start_vertices_count( ) const
{
	render::culling::spatial_sector const& s = m_graph->get_sectors( )[m_start_sector_id];
	return s.get_portals_count( );
}

template < typename vertex_type >
inline bool search_restrictor::accessible(
	vertex_id_type const&,
	vertex_type const& current_vertex,
	u32 const* const& edge_iterator
) const
{
	if ( *edge_iterator == current_vertex.id( ).portal_id )
		return false;

	render::culling::portal const& portal = m_graph->get_portals( )[current_vertex.id( ).portal_id];
	u32 const outcoming_sector_id = current_vertex.id( ).incoming_sector_index ? portal.get_sectors( )[0] : portal.get_sectors( )[1];
	if ( m_target_sector_id == outcoming_sector_id )
		return false;

	return true;
}

inline void search_restrictor::on_before_search( )
{
	m_different_paths_left = math::min( m_graph->get_sectors( )[m_target_sector_id].get_portals_count( ), 4u );
	m_vertex_ids.clear( );
}

template < typename priority_queue_impl_type >
inline bool search_restrictor::limit_reached(
	priority_queue_impl_type const& queue,
	u32 const iteration_count
) const
{
	if ( iteration_count >= m_max_iteration_count )
		return true;

	float const& current_range = queue.best( ).f( );
	if ( current_range >= m_max_range )
		return true;

	u32 const visited_vertex_count = queue.visited_vertex_count( );
	if ( visited_vertex_count >= m_max_visited_vertex_count )
		return true;

	if ( !m_different_paths_left )
		return true;

	return false;
}

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SOURCES_SEARCH_RESTRICTOR_H_INCLUDED
