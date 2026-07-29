////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FILL_SOURCE_VERTICES___L2_COLORED_STATIC_VERTEX_H_INCLUDED
#define RENDER_FILL_SOURCE_VERTICES___L2_COLORED_STATIC_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;
struct vostok::render::fill_source_vertices::__l2::static_vertex;

namespace vostok {
namespace render {

struct fill_source_vertices::__l2::colored_static_vertex : public fill_source_vertices::__l2::static_vertex {
	inline		colored_static_vertex	( ) { /* no source */ }

	/* 0x0000 */	/* fill_source_vertices::__l2::static_vertex */
	/* 0x0020 */	math::color		color_component;
}; // struct fill_source_vertices::__l2::colored_static_vertex

STATIC_SIZE_ASSERT(fill_source_vertices::__l2::colored_static_vertex, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FILL_SOURCE_VERTICES___L2_COLORED_STATIC_VERTEX_H_INCLUDED
