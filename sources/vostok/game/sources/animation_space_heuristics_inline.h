////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED
#define ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline animation_space_heuristics::animation_space_heuristics(
	animation_space_graph_ptr const&	graph,
	animation_space_vertex_id const&	target_vertex_id,
	const float							max_speed
) :
	// the ref/const members force the init-list even though the body is a stub
	m_graph( graph ),
	m_target_vertex_id( target_vertex_id ),
	m_max_speed( max_speed )
{
	// FUNCTION BODY[0xad4a0]: 2
	// <0xad4ab>|0x00b|+0x05f:'22'
	// <0xad50a>|0x06a|+0x00d:'23'
	// ******
}

// STATE[STUB]
inline float animation_space_heuristics::estimate(
	animation_space_vertex_id const* const	current_vertex_id_ptr,
	animation_space_vertex_id const&	neighbour_vertex_id
) const
{
	// LOCALS
	// const float 						result
	// ******

	return 0.0f;

	// FUNCTION BODY[0xad540]: 9
	// <0>
	// <1>
	// <0xad541>|0x001|+0x055:'47'
	// <0xad596>|0x056|+0x003:'48'
	// <0>
	// <0xad599>|0x059|+0x010:'50'
	// <0xad5a9>|0x069|+0x023:'51'
	// <0>
	// <1>
	// ******
}

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED
