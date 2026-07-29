////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DEVICE_CAPS_CAPS_GEOMETRY_H_INCLUDED
#define RENDER_DEVICE_CAPS_CAPS_GEOMETRY_H_INCLUDED

namespace vostok {
namespace render {

struct device_caps::caps_geometry {

	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 16, position: 0 } )*/	registers;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 16, position: 16 } )*/	instructions;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 0 } )*/	software;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 1 } )*/	point_sprites;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 2 } )*/	vtf;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 3 } )*/	npatches;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 4, position: 4 } )*/	clip_planes;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 8, position: 8 } )*/	vertex_cache;
}; // struct device_caps::caps_geometry

STATIC_SIZE_ASSERT(device_caps::caps_geometry, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DEVICE_CAPS_CAPS_GEOMETRY_H_INCLUDED
