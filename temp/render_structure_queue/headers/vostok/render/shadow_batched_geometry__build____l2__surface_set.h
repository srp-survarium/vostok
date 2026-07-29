////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADOW_BATCHED_GEOMETRY_BUILD___L2_SURFACE_SET_H_INCLUDED
#define RENDER_SHADOW_BATCHED_GEOMETRY_BUILD___L2_SURFACE_SET_H_INCLUDED

/* INCLUDES */
class vostok::render::render_surface;

namespace vostok {
namespace render {

struct shadow_batched_geometry::build::__l2::surface_set {
	inline		surface_set	( ) { /* no source */ }

	/* 0x0000 */	float4x4			transform;
	/* 0x0040 */	render_surface*		surface;
}; // struct shadow_batched_geometry::build::__l2::surface_set

STATIC_SIZE_ASSERT(shadow_batched_geometry::build::__l2::surface_set, 0x44);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADOW_BATCHED_GEOMETRY_BUILD___L2_SURFACE_SET_H_INCLUDED
