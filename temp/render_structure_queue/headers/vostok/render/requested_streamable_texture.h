////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REQUESTED_STREAMABLE_TEXTURE_H_INCLUDED
#define RENDER_REQUESTED_STREAMABLE_TEXTURE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct requested_streamable_texture {
	inline		requested_streamable_texture	( ) { /* no source */ }
	inline		~requested_streamable_texture	( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		path;
	/* 0x0110 */	res_texture_ptr			texture;
	/* 0x0114 */	u32						num_mips;
}; // struct requested_streamable_texture

STATIC_SIZE_ASSERT(requested_streamable_texture, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REQUESTED_STREAMABLE_TEXTURE_H_INCLUDED
