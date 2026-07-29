////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_CREATE_SHADOW_PASS_GEOMETRY___L2_OPT_STATIC_VERTEX_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_CREATE_SHADOW_PASS_GEOMETRY___L2_OPT_STATIC_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

/* FORWARD REFS */
class vostok::render::static_render_surface::create_shadow_pass_geometry::__l2::static_vertex0;

namespace vostok {
namespace render {

struct static_render_surface::create_shadow_pass_geometry::__l2::opt_static_vertex {
	inline	void	set					(
						static_render_surface::create_shadow_pass_geometry::__l2::static_vertex0 const&	arg_0
					) { /* no source */ }

	inline			opt_static_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	float2			uv;
}; // struct static_render_surface::create_shadow_pass_geometry::__l2::opt_static_vertex

STATIC_SIZE_ASSERT(static_render_surface::create_shadow_pass_geometry::__l2::opt_static_vertex, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_CREATE_SHADOW_PASS_GEOMETRY___L2_OPT_STATIC_VERTEX_H_INCLUDED
