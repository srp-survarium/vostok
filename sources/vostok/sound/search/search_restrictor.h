////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SEARCH_SEARCH_RESTRICTOR_H_INCLUDED
#define VOSTOK_SOUND_SEARCH_SEARCH_RESTRICTOR_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/sound/sound.h>
#include <vostok/sound/search/vertex_id_type.h>
#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace sound {
namespace search {

class search_restrictor : public noncopyable
{
public:
	inline search_restrictor(
		render::culling::portal_sector_structure_ptr const& graph,
		u32 const start_sector_id,
		u32 const target_sector_id,
		float3 const& source_position,
		float const& max_range,
		u32 const max_iteration_count,
		u32 const max_visited_vertex_count
	) :
		m_graph( graph ),
		m_start_sector_id( start_sector_id ),
		m_target_sector_id( target_sector_id ),
		m_source_position( source_position ),
		m_max_range( max_range ),
		m_max_iteration_count( max_iteration_count ),
		m_max_visited_vertex_count( max_visited_vertex_count ),
		m_different_paths_left( 0 )
	{
		R_ASSERT( m_graph );
	}

	inline vertex_id_type start_vertex_id( u32 const start_vertex_id ) const;
	inline bool target_reached( vertex_id_type const& vertex_id );
	inline u32 get_start_vertices_count( ) const;

	template < typename vertex_type >
	inline bool accessible(
		vertex_id_type const& neighbour_vertex_id,
		vertex_type const& current_vertex,
		u32 const* const& edge_iterator
	) const;

	inline void on_before_search( );

	inline void on_after_search( bool const ) const
	{
	}

	template < typename priority_queue_impl_type >
	inline bool limit_reached(
		priority_queue_impl_type const& queue,
		u32 const iteration_count
	) const;

	inline fixed_vector< vertex_id_type, 4 > const& vertex_ids( ) const
	{
		return m_vertex_ids;
	}

	inline ~search_restrictor( )
	{
	}

private:
	fixed_vector< vertex_id_type, 4 > m_vertex_ids;
	render::culling::portal_sector_structure_ptr const& m_graph;
	u32 const m_start_sector_id;
	u32 const m_target_sector_id;
	float3 const& m_source_position;
	float const m_max_range;
	u32 const m_max_iteration_count;
	u32 const m_max_visited_vertex_count;
	u32 m_different_paths_left;
}; // class search_restrictor

STATIC_SIZE_ASSERT( search_restrictor, 0x58 );

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SEARCH_SEARCH_RESTRICTOR_H_INCLUDED
