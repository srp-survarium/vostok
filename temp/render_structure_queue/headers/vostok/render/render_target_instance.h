////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_TARGET_INSTANCE_H_INCLUDED
#define RENDER_RENDER_TARGET_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<64>;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::render_target;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct render_target_instance {
	inline		render_target_instance	( ) { /* no source */ }
	inline		~render_target_instance	( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 64 >		orig_name;
	/* 0x004c */	fixed_string< 64 >		name;
	/* 0x0098 */	render_target_ptr		target;
	/* 0x009c */	res_texture_ptr			texture;
}; // struct render_target_instance

STATIC_SIZE_ASSERT(render_target_instance, 0xA0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_TARGET_INSTANCE_H_INCLUDED
