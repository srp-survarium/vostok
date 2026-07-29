////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LOD_RENDER_INFO_H_INCLUDED
#define RENDER_LOD_RENDER_INFO_H_INCLUDED

/* INCLUDES */
struct vostok::render::lod_entry;

namespace vostok {
namespace render {

struct lod_render_info {
	inline						lod_render_info	( ) { /* no source */ }

	inline	bool				has_geometry	( u32 arg_0 ) const { /* no source */ }

	inline	lod_entry const*	is_active		( u32 arg_0 ) const { /* no source */ }

	/* 0x0000 */	lod_entry*		lods;
	/* 0x0004 */	u32				num_lods;
}; // struct lod_render_info

STATIC_SIZE_ASSERT(lod_render_info, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LOD_RENDER_INFO_H_INCLUDED
