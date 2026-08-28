////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SEARCH_GRAPH_HEURISTICS_H_INCLUDED
#define VOSTOK_SOUND_SEARCH_GRAPH_HEURISTICS_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/sound/search/vertex_id_type.h>
#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace sound {
namespace search {

class graph_heuristics : private boost::noncopyable
{
public:
	inline graph_heuristics(
		render::culling::portal_sector_structure_ptr const& graph,
		float3 const& target_position
	) :
		m_target_position( target_position ),
		m_graph( graph )
	{
	}

	inline void on_before_search( )
	{
	}

	inline void on_after_search( bool const )
	{
	}

	inline void on_start_iteration( vertex_id_type const )
	{
	}

	inline float estimate(
		vertex_id_type const* const current_vertex_id_ptr,
		vertex_id_type const& neighbour_vertex_id
	) const;

	// dtor before evaluate: retail's method list orders it here
	inline ~graph_heuristics( )
	{
	}

	template < typename vertex_type >
	inline float evaluate(
		vertex_type const& current_vertex,
		vertex_type const& neighbour_vertex,
		u32 const* const& iterator
	) const;

	static inline bool metric_euclidian( )
	{
		return true;
	}

private:
	float3 const m_target_position;
	render::culling::portal_sector_structure_ptr const& m_graph;
}; // class graph_heuristics

STATIC_SIZE_ASSERT( graph_heuristics, 0x10 );

} // namespace search
} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SEARCH_GRAPH_HEURISTICS_H_INCLUDED
