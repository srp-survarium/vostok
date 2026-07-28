////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_GRAPH_WRAPPER_INLINE_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_WRAPPER_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// claude@NOTE: 6-statement pure-math A* neighbour-pose composer (structure @0xad280,
// 1 local: float4x4 temp). NO lexeme wall - all math. Decoded shape (--view target):
//   line 44: float4x4 temp = math::create_matrix( math::quaternion( <zero xyz> ), <pos> );
//   line 45: read vertex_id.translation ( the const& arg, ecx +0x10/+0x18 );
//   line 48: index get_edges()[iterator] - addr = graph + animations_count*0x124 +
//            mixes_count*8 + 0x120 + iterator*0x28 (edge stride 0x28); edge.movement is
//            the vertex_id at edge+0 (rotation @+0, translation @+0x10);
//   line 49: result.translation = edge.movement.translation * temp  (float3*float4x4,
//            the 4x3 transform at 0x5f-0x114: x*row0 + z*row2 + y*row1 + row3);
//   line 50: result.rotation = vertex_id.rotation * edge.movement.rotation  (quaternion
//            operator*, the cross/dot expansion at 0x117-0x212);
//   line 51: return result.
// PARKED buildability return. OPEN: the exact create_matrix arguments are ambiguous - the
// quaternion arg writes only 3 zeroed floats (xyz, w untouched), so it is NOT quaternion()
// (no-op) nor quaternion(float3(0,0,0)) (would set w=-1 via sin/cos); the position arg is
// not visibly loaded into a register before the call (same convention puzzle as
// debug_draw's create_matrix). NEXT STEP: write the decoded body using math::create_matrix +
// float3 operator*(float4x4) + quaternion operator* (all available) and iterate the
// create_matrix(q,pos) spelling + the operand order off --view diff. Reachable through the
// A* search-service wrapper (animations_search_service).
inline animation_space_vertex_id animation_space_graph_wrapper::vertex_id( animation_space_vertex_id const& vertex_id, const u32 iterator ) const
{
	return animation_space_vertex_id( );
}

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_GRAPH_WRAPPER_INLINE_H_INCLUDED
