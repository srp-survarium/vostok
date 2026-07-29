#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

class render_model_instance_impl : public resources::unmanaged_resource {
public:
	render_model_instance_impl( );
	virtual ~render_model_instance_impl( ) { }
};

typedef resources::resource_ptr<
	render_model_instance_impl,
	resources::unmanaged_intrusive_base
> render_model_instance_impl_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED
