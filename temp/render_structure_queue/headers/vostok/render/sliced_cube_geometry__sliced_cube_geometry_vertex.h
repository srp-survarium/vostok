////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SLICED_CUBE_GEOMETRY_SLICED_CUBE_GEOMETRY_VERTEX_H_INCLUDED
#define RENDER_SLICED_CUBE_GEOMETRY_SLICED_CUBE_GEOMETRY_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct sliced_cube_geometry::sliced_cube_geometry_vertex {
	inline		sliced_cube_geometry_vertex	( ) { /* no source */ }

	/* 0x0000 */	float4		position;
	/* 0x0010 */	float4		xy_and_slice_index;
}; // struct sliced_cube_geometry::sliced_cube_geometry_vertex

STATIC_SIZE_ASSERT(sliced_cube_geometry::sliced_cube_geometry_vertex, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SLICED_CUBE_GEOMETRY_SLICED_CUBE_GEOMETRY_VERTEX_H_INCLUDED
