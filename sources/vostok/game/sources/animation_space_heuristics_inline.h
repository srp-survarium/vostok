////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED
#define ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED

namespace survarium {

// claude@NOTE: these inline COMDATs (ctor + estimate, and the related operator!= in
// animation_space_graph.h) only emit when a_star::find<...> is instantiated, which
// happens in animations_search_service::search. That fn is still a STUB (parked on the
// A* graph_wrapper/vertex_manager machinery), so our base never reaches these and they
// stay UNPAIRED despite the bodies being recovered from the target asm. They pair once
// search() compiles - do NOT delete them.

inline animation_space_heuristics::animation_space_heuristics(
	animation_space_graph_ptr const&	graph,
	animation_space_vertex_id const&	target_vertex_id,
	const float							max_speed
) :
	m_graph( graph ),
	m_target_vertex_id( target_vertex_id ),
	m_best_vertex_id( ),
	m_min_heuristics_value( math::infinity ),
	m_max_speed( max_speed )
{
}

inline float animation_space_heuristics::estimate(
	animation_space_vertex_id const* const	current_vertex_id_ptr,
	animation_space_vertex_id const&	neighbour_vertex_id
) const
{
	const float result = ( neighbour_vertex_id.translation - m_target_vertex_id.translation ).length( ) / m_max_speed;

	if ( result < m_min_heuristics_value )
	{
		m_min_heuristics_value	= result;
		m_best_vertex_id		= neighbour_vertex_id;
	}

	return result;
}

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED
