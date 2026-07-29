////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BATCHED_VERTEX_SOURCE_H_INCLUDED
#define RENDER_BATCHED_VERTEX_SOURCE_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct batched_vertex_source {
	inline		batched_vertex_source	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	math::color		tangent;
	/* 0x0014 */	math::color		binormal;
	/* 0x0018 */	math::color		clr;
	/* 0x001c */	float2			uv;
}; // struct batched_vertex_source

STATIC_SIZE_ASSERT(batched_vertex_source, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BATCHED_VERTEX_SOURCE_H_INCLUDED
