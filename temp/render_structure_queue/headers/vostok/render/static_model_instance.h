////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_MODEL_INSTANCE_H_INCLUDED
#define RENDER_STATIC_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::render::render_model_instance;

namespace vostok {
namespace render {

class static_model_instance : public resources::unmanaged_resource {
public:
	inline		static_model_instance	( ) { /* no source */ }
	virtual		~static_model_instance	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render_model_instance_ptr				m_render_model;
	/* 0x010c */	resources::unmanaged_resource_ptr		m_sound_environment;
}; // class static_model_instance

STATIC_SIZE_ASSERT(static_model_instance, 0x110);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_MODEL_INSTANCE_H_INCLUDED
