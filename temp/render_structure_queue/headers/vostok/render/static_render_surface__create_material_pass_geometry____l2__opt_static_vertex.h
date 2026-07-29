////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_CREATE_MATERIAL_PASS_GEOMETRY___L2_OPT_STATIC_VERTEX_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_CREATE_MATERIAL_PASS_GEOMETRY___L2_OPT_STATIC_VERTEX_H_INCLUDED

/* FORWARD REFS */
class vostok::render::static_render_surface::create_material_pass_geometry::__l2::static_vertex0;

namespace vostok {
namespace render {

struct static_render_surface::create_material_pass_geometry::__l2::opt_static_vertex {
	inline	void	set					(
						static_render_surface::create_material_pass_geometry::__l2::static_vertex0 const&	arg_0
					) { /* no source */ }

	inline			opt_static_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	float2		uv;
	/* 0x0014 */	u8			binormal;
}; // struct static_render_surface::create_material_pass_geometry::__l2::opt_static_vertex

STATIC_SIZE_ASSERT(static_render_surface::create_material_pass_geometry::__l2::opt_static_vertex, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_CREATE_MATERIAL_PASS_GEOMETRY___L2_OPT_STATIC_VERTEX_H_INCLUDED
