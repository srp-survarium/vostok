////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_FILL_LPV_VERTEX_COLOR___L2_STATIC_VERTEX0_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_FILL_LPV_VERTEX_COLOR___L2_STATIC_VERTEX0_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0 {
	inline		static_vertex0	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	math::color		tangent;
	/* 0x0014 */	math::color		binormal;
	/* 0x0018 */	float2			uv;
}; // struct static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0

STATIC_SIZE_ASSERT(static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_FILL_LPV_VERTEX_COLOR___L2_STATIC_VERTEX0_H_INCLUDED
