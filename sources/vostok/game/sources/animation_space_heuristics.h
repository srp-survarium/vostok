// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_SPACE_HEURISTICS_H_INCLUDED
#define ANIMATION_SPACE_HEURISTICS_H_INCLUDED

#include "animation_space_graph.h"

namespace survarium {

class animation_space_heuristics : private boost::noncopyable {
public:
											animation_space_heuristics	(
													animation_space_graph_ptr const&	graph,
													animation_space_vertex_id const&	target_vertex_id,
													const float							max_speed
												);

	inline	void								on_before_search			( ) { /* no source */ }
	inline	void								on_after_search				( const bool arg_0 ) { /* no source */ }
	inline	void								on_start_iteration			( const animation_space_vertex_id arg_0 ) { /* no source */ }

			float								estimate					(
													animation_space_vertex_id const* const	current_vertex_id_ptr,
													animation_space_vertex_id const&	neighbour_vertex_id
												) const;
	template < typename VertexType >
	inline	float							evaluate					(
													VertexType const& current_vertex,
													VertexType const& neighbour_vertex,
													const u32 iterator
												) const
	{
		return current_vertex.g( ) + m_graph->edge( iterator ).animation_length;
	}

	static	inline	bool						metric_euclidian			( ) { return true; }
	inline	animation_space_vertex_id const&	best_vertex_id				( ) const { /* no source */ return m_best_vertex_id; }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	animation_space_graph_ptr const&	m_graph;
	/* 0x0004 */	animation_space_vertex_id const&	m_target_vertex_id;
	/* 0x0008 */	mutable	animation_space_vertex_id	m_best_vertex_id;
	/* 0x0024 */	mutable	float						m_min_heuristics_value;
	/* 0x0028 */	const float							m_max_speed;
}; // class animation_space_heuristics

STATIC_SIZE_ASSERT(animation_space_heuristics, 0x2C);

} // namespace survarium

#include "animation_space_heuristics_inline.h"

#endif // #ifndef ANIMATION_SPACE_HEURISTICS_H_INCLUDED
