////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TRACER_MODEL_INSTANCE_H_INCLUDED
#define RENDER_TRACER_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::math::color;
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
class vostok::render::render_model_instance;

namespace vostok {
namespace render {

struct tracer_model_instance : public resources::unmanaged_resource {
	inline	void	set_color				( math::color const& arg_0 ) { /* no source */ }

	inline			tracer_model_instance	( ) { /* no source */ }
	virtual			~tracer_model_instance	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	float4x4						m_transform;
	/* 0x0148 */	render_model_instance_ptr		m_model;
	/* 0x014c */	math::color						m_color;
}; // struct tracer_model_instance

STATIC_SIZE_ASSERT(tracer_model_instance, 0x150);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TRACER_MODEL_INSTANCE_H_INCLUDED
