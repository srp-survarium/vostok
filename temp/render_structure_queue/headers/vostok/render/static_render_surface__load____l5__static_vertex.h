////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_LOAD___L5_STATIC_VERTEX_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_LOAD___L5_STATIC_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct static_render_surface::load::__l5::static_vertex {
	inline		static_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	math::color		tangent;
	/* 0x0014 */	math::color		binormal;
	/* 0x0018 */	float2			uv;
}; // struct static_render_surface::load::__l5::static_vertex

STATIC_SIZE_ASSERT(static_render_surface::load::__l5::static_vertex, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_LOAD___L5_STATIC_VERTEX_H_INCLUDED
