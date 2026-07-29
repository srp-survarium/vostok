////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_NAMED_INSTANCE_H_INCLUDED
#define RENDER_TEXTURE_NAMED_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct texture_named_instance {
	inline		texture_named_instance	( ) { /* no source */ }

	/* 0x0000 */	res_texture*			texture;
	/* 0x0004 */	fixed_string< 260 >		path;
}; // struct texture_named_instance

STATIC_SIZE_ASSERT(texture_named_instance, 0x114);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_NAMED_INSTANCE_H_INCLUDED
