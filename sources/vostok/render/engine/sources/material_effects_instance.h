#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

class material_effects_instance : public resources::unmanaged_resource {
public:
	virtual ~material_effects_instance( ) { }
};

typedef resources::resource_ptr<
	material_effects_instance,
	resources::unmanaged_intrusive_base
> material_effects_instance_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED
