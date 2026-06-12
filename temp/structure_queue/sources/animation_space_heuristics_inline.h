////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\GAME\SOURCES\ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED
#define VOSTOK\GAME\SOURCES\ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline  animation_space_heuristics::animation_space_heuristics(
	animation_space_graph_ptr const&	graph,
	animation_space_vertex_id const&	target_vertex_id,
	const float							max_speed
)
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

	// TYPEDEFS
	// typedef
	// 	float
	// 	distance_type;

	// typedef
	// 	survarium::animation_space_heuristics
	// 	path_heuristics_type;

	// typedef
	// 	survarium::animation_space_search_restrictor
	// 	search_restrictor_type;

	// typedef
	// 	survarium::animation_space_vertex_id
	// 	vertex_id_type;

	// typedef
	// 	survarium::animations_search_service::vertex_manager_impl_type
	// 	vertex_manager_type;

	// typedef
	// 	survarium::animations_search_service::vertex_type
	// 	vertex_type;

	// typedef
	// 	survarium::vector< u32 >
	// 	path_type;

	// typedef
	// 	u32*
	// 	iterator_type;

	// typedef
	// 	vostok::ai::path_constructor::edge::impl< survarium::animations_search_service::vertex_type, survarium::vector< u32 > >
	// 	path_constructor_type;

	// ******

} // namespace survarium

#endif // #ifndef VOSTOK\GAME\SOURCES\ANIMATION_SPACE_HEURISTICS_INLINE_H_INCLUDED
