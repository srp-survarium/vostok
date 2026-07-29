////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ANIMATED_MODEL_INSTANCE_H_INCLUDED
#define RENDER_ANIMATED_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<32>;
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::render::skin,vostok::resources::unmanaged_intrusive_base>
	vostok::render::skin_ptr;
class vostok::render::render_model_instance;
class vostok::render::skin;

namespace vostok {
namespace render {

struct animated_model_instance : public resources::unmanaged_resource {
	inline		animated_model_instance	( ) { /* no source */ }
	virtual		~animated_model_instance( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render_model_instance_ptr		m_model;
	/* 0x010c */	skin_ptr						m_skin;
	/* 0x0110 */	fixed_string< 32 >				m_hit_params;
}; // struct animated_model_instance

STATIC_SIZE_ASSERT(animated_model_instance, 0x140);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ANIMATED_MODEL_INSTANCE_H_INCLUDED
