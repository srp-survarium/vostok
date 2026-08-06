#ifndef VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_H_INCLUDED

#include <vostok/math_color.h>
#include <vostok/render/facade/model.h>

namespace vostok {
namespace render {

struct tracer_model_instance : public resources::unmanaged_resource {
public:
	void set_color( math::color const& color ) { m_color = color; }

	float4x4 m_transform;
	render_model_instance_ptr m_model;
	math::color m_color;
};

STATIC_SIZE_ASSERT( tracer_model_instance, 0x150 );

typedef resources::resource_ptr<
	tracer_model_instance,
	resources::unmanaged_intrusive_base
> tracer_model_instance_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_H_INCLUDED
