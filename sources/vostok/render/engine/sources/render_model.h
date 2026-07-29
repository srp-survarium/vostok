#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

class render_surface;
struct model_locator_item;
struct model_lods_descriptor;

class render_model : public resources::unmanaged_resource {
public:
	render_model( );
	virtual ~render_model( );

	virtual void load_properties( configs::binary_config_value const& properties );
	virtual void set_children( render_surface** children_in, u8 count, model_lods_descriptor* lods );
	virtual bool get_locator( pcstr locator_name, model_locator_item& result ) const;
};

typedef resources::resource_ptr<
	render_model,
	resources::unmanaged_intrusive_base
> render_model_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_H_INCLUDED
