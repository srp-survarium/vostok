////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DEVICE_CAPS_CAPS_RASTER_H_INCLUDED
#define RENDER_DEVICE_CAPS_CAPS_RASTER_H_INCLUDED

namespace vostok {
namespace render {

struct device_caps::caps_raster {

	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 16, position: 0 } )*/	registers;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 16, position: 16 } )*/	instructions;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 4, position: 0 } )*/	stages;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 4, position: 4 } )*/	mrt_count;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 8 } )*/	b_mrt_mixdepth;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 9 } )*/	b_non_pow2;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 10 } )*/	b_cubemap;
}; // struct device_caps::caps_raster

STATIC_SIZE_ASSERT(device_caps::caps_raster, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DEVICE_CAPS_CAPS_RASTER_H_INCLUDED
