////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LOD_ENTRY_H_INCLUDED
#define RENDER_LOD_ENTRY_H_INCLUDED

namespace vostok {
namespace render {

struct lod_entry {
	inline		lod_entry	( ) { /* no source */ }

	/* 0x0000 */	u32		start_index;
	/* 0x0004 */	u32		num_indices;
}; // struct lod_entry

STATIC_SIZE_ASSERT(lod_entry, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LOD_ENTRY_H_INCLUDED
