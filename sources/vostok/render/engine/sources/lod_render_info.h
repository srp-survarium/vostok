#ifndef VOSTOK_RENDER_ENGINE_LOD_RENDER_INFO_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LOD_RENDER_INFO_H_INCLUDED

#include "lod_entry.h"

namespace vostok {
namespace render {

struct lod_render_info {
	lod_render_info( ) : lods( 0 ), num_lods( 0 ) { }

	bool has_geometry( u32 lod_index ) const
	{
		return lods[lod_index].num_indices != 0;
	}

	lod_entry const* is_active( u32 lod_index ) const
	{
		return has_geometry( lod_index ) ? &lods[lod_index] : 0;
	}

	lod_entry* lods;
	u32 num_lods;
}; // struct lod_render_info

STATIC_SIZE_ASSERT(lod_render_info, 0x8);

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_LOD_RENDER_INFO_H_INCLUDED
