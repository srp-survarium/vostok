#include "pch.h"
#include "model_cooker.h"
#include <vostok/render/facade/model.h>

namespace vostok {

namespace configs {
class binary_config_value;
}

namespace sound {
struct world;
}

namespace render {

struct static_model_instance_user_data {
	static_model_instance_user_data( ) { }
	~static_model_instance_user_data( ) { }

	configs::binary_config_value const* config;
	sound::world* sound_world;
	resources::unmanaged_resource_ptr sound_scene;
};

// STATE[STUB]
static_model_instance_cook::static_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::static_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76c2d0]
}

// STATE[STUB]
void static_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// FUNCTION BODY[0x76cd40]
}

// STATE[STUB]
void static_model_instance_cook::on_subresources_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*
)
{
	// FUNCTION BODY[0x76c7a0]
}

// STATE[STUB]
void static_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x76c380]
	DELETE( resource );
}

// STATE[STUB]
skeleton_model_instance_cook::skeleton_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76c260]
}

// STATE[STUB]
void skeleton_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// FUNCTION BODY[0x76cb10]
}

// STATE[STUB]
void skeleton_model_instance_cook::on_skeleton_config_loaded(
	resources::queries_result&,
	skeleton_model_instance_cook_data*
)
{
	// FUNCTION BODY[0x76c9c0]
}

// STATE[STUB]
void skeleton_model_instance_cook::on_skeleton_loaded(
	resources::queries_result&,
	skeleton_model_instance_cook_data*
)
{
	// FUNCTION BODY[0x76c6b0]
}

// STATE[STUB]
void skeleton_model_instance_cook::on_render_model_loaded(
	resources::queries_result&,
	skeleton_model_instance_cook_data*
)
{
	// FUNCTION BODY[0x76c5c0]
}

// STATE[STUB]
void skeleton_model_instance_cook::on_all_subresources_ready(
	skeleton_model_instance_cook_data*
)
{
	// FUNCTION BODY[0x76c3c0]
}

// STATE[STUB]
void skeleton_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x76c340]
	DELETE( resource );
}

} // namespace render
} // namespace vostok
