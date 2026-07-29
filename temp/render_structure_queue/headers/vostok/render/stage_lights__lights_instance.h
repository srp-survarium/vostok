////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_LIGHTS_LIGHTS_INSTANCE_H_INCLUDED
#define RENDER_STAGE_LIGHTS_LIGHTS_INSTANCE_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::untyped_buffer;

namespace vostok {
namespace render {

struct stage_lights::lights_instance {
	inline		lights_instance	( ) { /* no source */ }
	inline		~lights_instance( ) { /* no source */ }

	/* 0x0000 */	untyped_buffer_ptr		m_instance_vb;
}; // struct stage_lights::lights_instance

STATIC_SIZE_ASSERT(stage_lights::lights_instance, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_LIGHTS_LIGHTS_INSTANCE_H_INCLUDED
