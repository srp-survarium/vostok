////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADOW_VERTEX_H_INCLUDED
#define RENDER_SHADOW_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct shadow_vertex {
	inline		shadow_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	float3		object_position;
	/* 0x0018 */	float2		uv;
}; // struct shadow_vertex

STATIC_SIZE_ASSERT(shadow_vertex, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADOW_VERTEX_H_INCLUDED
