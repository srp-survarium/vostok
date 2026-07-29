////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STREAMING_TEXTURE_INSTANCE_H_INCLUDED
#define RENDER_STREAMING_TEXTURE_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::math::sphere;
struct vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct streaming_texture_instance {
	inline		streaming_texture_instance	( ) { /* no source */ }

	/* 0x0000 */	math::sphere				object_sphere;
	/* 0x0010 */	float						texel_factor;
	/* 0x0014 */	render_surface_instance*	surface_instance;
}; // struct streaming_texture_instance

STATIC_SIZE_ASSERT(streaming_texture_instance, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STREAMING_TEXTURE_INSTANCE_H_INCLUDED
