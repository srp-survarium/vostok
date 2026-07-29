////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_STRUCT_H_INCLUDED
#define RENDER_VERTEX_STRUCT_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_struct {
	inline		vertex_struct	( ) { /* no source */ }

	/* 0x0000 */	float4		bound;
	/* 0x0010 */	float2		pixel;
}; // struct vertex_struct

STATIC_SIZE_ASSERT(vertex_struct, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_STRUCT_H_INCLUDED
