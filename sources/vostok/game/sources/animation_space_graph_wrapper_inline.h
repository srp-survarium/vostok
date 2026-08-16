////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_GRAPH_WRAPPER_INLINE_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_WRAPPER_INLINE_H_INCLUDED

namespace survarium {

inline animation_space_vertex_id animation_space_graph_wrapper::vertex_id( animation_space_vertex_id const& vertex_id, const u32 iterator ) const
{
	float4x4					temp	= math::create_rotation( vertex_id.rotation );
	temp.c.xyz( )						= vertex_id.translation;

	animation_space_vertex_id	result;
	animation_space_edge const&	edge	= m_graph->get_edges( )[ iterator ];
	result.translation					= edge.movement.translation * temp;
	result.rotation						= edge.movement.rotation * vertex_id.rotation;
	return								result;
}

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_GRAPH_WRAPPER_INLINE_H_INCLUDED
