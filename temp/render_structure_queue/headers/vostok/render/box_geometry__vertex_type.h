////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BOX_GEOMETRY_VERTEX_TYPE_H_INCLUDED
#define RENDER_BOX_GEOMETRY_VERTEX_TYPE_H_INCLUDED

namespace vostok {
namespace render {

struct box_geometry::vertex_type {
	inline		vertex_type	( ) { /* no source */ }

	/* 0x0000 */	float4		position;
}; // struct box_geometry::vertex_type

STATIC_SIZE_ASSERT(box_geometry::vertex_type, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BOX_GEOMETRY_VERTEX_TYPE_H_INCLUDED
