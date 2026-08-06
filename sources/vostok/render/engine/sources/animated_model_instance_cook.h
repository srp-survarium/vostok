#ifndef VOSTOK_RENDER_ENGINE_ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED

#include <vostok/configs_binary_config.h>
#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

class animated_model_instance_cook : public resources::translate_query_cook {
public:
	animated_model_instance_cook( );

	virtual void translate_request_path(
		pcstr							request,
		fs_new::virtual_path_string&	new_request
	) const;

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_config_loaded( resources::queries_result& data );

	void on_skeleton_config_loaded(
		resources::queries_result&	data,
		configs::binary_config_ptr	config
	);

	void on_subresources_loaded(
		resources::queries_result&	data,
		configs::binary_config_ptr	config
	);

public:
	virtual ~animated_model_instance_cook( ) { }
};

STATIC_SIZE_ASSERT( animated_model_instance_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED
