#ifndef RENDER_LOD_RENDER_INFO_H_INCLUDED
#define RENDER_LOD_RENDER_INFO_H_INCLUDED

#include "lod_entry.h"

namespace vostok {
namespace render {

struct lod_render_info {
	inline						lod_render_info	( ) : lods( NULL ), num_lods( 0 ) { /* no source */ }

	inline	bool				has_geometry	( u32 arg_0 ) const { return false; }

	inline	lod_entry const*	is_active		( u32 arg_0 ) const { return NULL; }

	/* 0x0000 */	lod_entry*		lods;
	/* 0x0004 */	u32				num_lods;
}; // struct lod_render_info

STATIC_SIZE_ASSERT(lod_render_info, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LOD_RENDER_INFO_H_INCLUDED
