////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TERRAIN_DATA_H_INCLUDED
#define RENDER_TERRAIN_DATA_H_INCLUDED

namespace vostok {
namespace render {

struct terrain_data {
	inline		terrain_data( ) { /* no source */ }

	/* 0x0000 */	float		height;
	/* 0x0004 */	float2		tex_shift;
	/* 0x000c */	u8			tex_id0;
	/* 0x000d */	u8			tex_id1;
	/* 0x000e */	u8			tex_id2;
	/* 0x000f */	u8			alpha0;
	/* 0x0010 */	u8			alpha1;
	/* 0x0011 */	u8			alpha2;
	/* 0x0012 */	u32			color;
}; // struct terrain_data

STATIC_SIZE_ASSERT(terrain_data, 0x16);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TERRAIN_DATA_H_INCLUDED
