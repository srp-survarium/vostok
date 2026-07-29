////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_POOL_SLOT_H_INCLUDED
#define RENDER_TEXTURE_POOL_SLOT_H_INCLUDED

/* INCLUDES */
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct texture_pool::slot {
	inline	explicit	slot( res_texture* arg_0 ) { /* no source */ }

	/* 0x0000 */	bool			occupied;
	/* 0x0004 */	res_texture*	texture;
}; // struct texture_pool::slot

STATIC_SIZE_ASSERT(texture_pool::slot, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_POOL_SLOT_H_INCLUDED
