////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_SOURCE_VERTEX_H_INCLUDED
#define RENDER_GRASS_SOURCE_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct grass_source_vertex {
	inline		grass_source_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	math::color		tangent;
	/* 0x0014 */	math::color		binormal;
	/* 0x0018 */	float2			uv;
}; // struct grass_source_vertex

STATIC_SIZE_ASSERT(grass_source_vertex, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_SOURCE_VERTEX_H_INCLUDED
