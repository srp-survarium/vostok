#include "pch.h"
#include "animated_model_instance_cook.h"

namespace vostok {
namespace render {

animated_model_instance_cook::animated_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::render_animated_model_instance_class,
		reuse_false,
		use_resource_manager_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70cdd0]
}

void animated_model_instance_cook::translate_request_path(
	pcstr							request,
	fs_new::virtual_path_string&	new_request
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x70ce30]
	new_request = request;
}

void animated_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70d4a0]
}

void animated_model_instance_cook::on_config_loaded( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70d2c0]
}

void animated_model_instance_cook::on_skeleton_config_loaded(
	resources::queries_result&,
	configs::binary_config_ptr
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70d090]
}

void animated_model_instance_cook::on_subresources_loaded(
	resources::queries_result&,
	configs::binary_config_ptr
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70ce50]
}

void animated_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70cd90]
	DELETE( resource );
}

} // namespace render
} // namespace vostok
