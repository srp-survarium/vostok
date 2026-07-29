////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_SECTOR_SYSTEM_QUAD_H_INCLUDED
#define RENDER_CULLING_PORTAL_SECTOR_SYSTEM_QUAD_H_INCLUDED

namespace vostok {
namespace render {
namespace culling {

struct portal_sector_system::quad {
	inline		quad( ) { /* no source */ }

	/* 0x0000 */	float3		vertices[4];
}; // struct portal_sector_system::quad

STATIC_SIZE_ASSERT(portal_sector_system::quad, 0x30);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_SECTOR_SYSTEM_QUAD_H_INCLUDED
