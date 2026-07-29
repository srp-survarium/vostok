////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FILL_SOURCE_VERTICES___L2_STATIC_VERTEX_H_INCLUDED
#define RENDER_FILL_SOURCE_VERTICES___L2_STATIC_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct fill_source_vertices::__l2::static_vertex {
	inline		static_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	math::color		tangent;
	/* 0x0014 */	math::color		binormal;
	/* 0x0018 */	float2			uv;
}; // struct fill_source_vertices::__l2::static_vertex

STATIC_SIZE_ASSERT(fill_source_vertices::__l2::static_vertex, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FILL_SOURCE_VERTICES___L2_STATIC_VERTEX_H_INCLUDED
