#ifndef VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

class tracer_model_instance : public resources::unmanaged_resource {
public:
	virtual ~tracer_model_instance( ) { }
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_H_INCLUDED
