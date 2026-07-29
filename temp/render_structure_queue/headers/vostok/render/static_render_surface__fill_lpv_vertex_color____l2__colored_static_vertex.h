////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_FILL_LPV_VERTEX_COLOR___L2_COLORED_STATIC_VERTEX_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_FILL_LPV_VERTEX_COLOR___L2_COLORED_STATIC_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;
struct vostok::render::static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0;

namespace vostok {
namespace render {

struct static_render_surface::fill_lpv_vertex_color::__l2::colored_static_vertex : public static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0 {
	inline		colored_static_vertex	( ) { /* no source */ }

	/* 0x0000 */	/* static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0 */
	/* 0x0020 */	math::color		color_component;
}; // struct static_render_surface::fill_lpv_vertex_color::__l2::colored_static_vertex

STATIC_SIZE_ASSERT(static_render_surface::fill_lpv_vertex_color::__l2::colored_static_vertex, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_FILL_LPV_VERTEX_COLOR___L2_COLORED_STATIC_VERTEX_H_INCLUDED
