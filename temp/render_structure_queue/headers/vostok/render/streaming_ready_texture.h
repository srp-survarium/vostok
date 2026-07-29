////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STREAMING_READY_TEXTURE_H_INCLUDED
#define RENDER_STREAMING_READY_TEXTURE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::render::res_texture;
class vostok::resources::managed_resource;

namespace vostok {
namespace render {

struct streaming_ready_texture {
	inline		streaming_ready_texture	( ) { /* no source */ }
	inline		~streaming_ready_texture( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >					name;
	/* 0x0110 */	res_texture_ptr						texture;
	/* 0x0114 */	resources::managed_resource_ptr		data;
	/* 0x0118 */	u32									num_mips;
	/* 0x011c */	float								distance;
}; // struct streaming_ready_texture

STATIC_SIZE_ASSERT(streaming_ready_texture, 0x120);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STREAMING_READY_TEXTURE_H_INCLUDED
