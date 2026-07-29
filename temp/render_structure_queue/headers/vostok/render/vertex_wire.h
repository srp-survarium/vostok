////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_WIRE_H_INCLUDED
#define RENDER_VERTEX_WIRE_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_wire {
	inline		vertex_wire	( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	float3		normal;
	/* 0x0018 */	float3		tangent;
	/* 0x0024 */	float3		binormal;
	/* 0x0030 */	float2		uv;
}; // struct vertex_wire

STATIC_SIZE_ASSERT(vertex_wire, 0x38);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_WIRE_H_INCLUDED
