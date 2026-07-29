////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_DOME_GEOMETRY_VERTEX_TYPE_H_INCLUDED
#define RENDER_SKY_DOME_GEOMETRY_VERTEX_TYPE_H_INCLUDED

namespace vostok {
namespace render {

struct sky_dome_geometry::vertex_type {
	inline		vertex_type	( ) { /* no source */ }

	/* 0x0000 */	float4		position;
	/* 0x0010 */	float2		uv;
}; // struct sky_dome_geometry::vertex_type

STATIC_SIZE_ASSERT(sky_dome_geometry::vertex_type, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_DOME_GEOMETRY_VERTEX_TYPE_H_INCLUDED
