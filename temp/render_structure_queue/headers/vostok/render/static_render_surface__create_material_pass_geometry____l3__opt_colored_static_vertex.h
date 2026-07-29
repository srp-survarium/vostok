////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_CREATE_MATERIAL_PASS_GEOMETRY___L3_OPT_COLORED_STATIC_VERTEX_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_CREATE_MATERIAL_PASS_GEOMETRY___L3_OPT_COLORED_STATIC_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;
struct vostok::render::static_render_surface::create_material_pass_geometry::__l2::opt_static_vertex;

/* FORWARD REFS */
class vostok::render::static_render_surface::create_material_pass_geometry::__l2::colored_static_vertex;

namespace vostok {
namespace render {

struct static_render_surface::create_material_pass_geometry::__l3::opt_colored_static_vertex : public static_render_surface::create_material_pass_geometry::__l2::opt_static_vertex {
	inline	void	set							(
						static_render_surface::create_material_pass_geometry::__l2::colored_static_vertex const&	arg_0
					) { /* no source */ }

	inline			opt_colored_static_vertex	( ) { /* no source */ }

	/* 0x0000 */	/* static_render_surface::create_material_pass_geometry::__l2::opt_static_vertex */
	/* 0x0018 */	math::color		color_component;
}; // struct static_render_surface::create_material_pass_geometry::__l3::opt_colored_static_vertex

STATIC_SIZE_ASSERT(static_render_surface::create_material_pass_geometry::__l3::opt_colored_static_vertex, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_CREATE_MATERIAL_PASS_GEOMETRY___L3_OPT_COLORED_STATIC_VERTEX_H_INCLUDED
