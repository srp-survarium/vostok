////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_PATCH_SORT_INFO_H_INCLUDED
#define RENDER_GRASS_PATCH_SORT_INFO_H_INCLUDED

namespace vostok {
namespace render {

struct grass_patch::sort_info {
	inline		sort_info	( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	u32			index_offset;
	/* 0x0010 */	u32			num_indices;
}; // struct grass_patch::sort_info

STATIC_SIZE_ASSERT(grass_patch::sort_info, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_PATCH_SORT_INFO_H_INCLUDED
