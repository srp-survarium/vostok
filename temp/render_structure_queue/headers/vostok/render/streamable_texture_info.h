////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STREAMABLE_TEXTURE_INFO_H_INCLUDED
#define RENDER_STREAMABLE_TEXTURE_INFO_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;
class vostok::render::vector<vostok::render::streaming_texture_instance>;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct streamable_texture_info {
	inline		streamable_texture_info	( ) { /* no source */ }
	inline		~streamable_texture_info( ) { /* no source */ }

	/* 0x0000 */	vector< streaming_texture_instance >	instances;
	/* 0x000c */	fixed_string< 260 >		path;
	/* 0x011c */	res_texture_ptr			texture;
}; // struct streamable_texture_info

STATIC_SIZE_ASSERT(streamable_texture_info, 0x120);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STREAMABLE_TEXTURE_INFO_H_INCLUDED
