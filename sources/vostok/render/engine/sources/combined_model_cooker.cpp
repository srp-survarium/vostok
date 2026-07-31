#include "pch.h"

// claude@NOTE: legacy-harvest disposition: skeleton_combined_model_cook is new-in-target (no temp/render_legacy ancestor; the legacy composite_render_model cooks are a different, retired subsystem) - matcher-phase work.
#include "combined_model_cooker.h"

namespace vostok {
namespace render {

skeleton_combined_model_cook::skeleton_combined_model_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_combined_model_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f2a0]
}

void skeleton_combined_model_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x650810]
}

void build_from_config( configs::binary_config_ptr&, skeleton_combined_cook_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f3e0]
}

void skeleton_combined_model_cook::on_config_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x650730]
}

void skeleton_combined_model_cook::query_resources_by_data(
	resources::query_result_for_cook*,
	skeleton_combined_cook_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6504d0]
}

void skeleton_combined_model_cook::on_resources_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*,
	skeleton_combined_cook_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64fe70]
}

void skeleton_combined_model_cook::on_material_effects_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*,
	skeleton_combined_cook_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f930]
}

void skeleton_combined_model_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f3a0]
	DELETE( resource );
}

skeleton_combined_render_model_instance_cook::
skeleton_combined_render_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_combined_render_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f230]
}

void skeleton_combined_render_model_instance_cook::translate_query(
	resources::query_result_for_cook&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64fd70]
}

void skeleton_combined_render_model_instance_cook::on_resources_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f820]
}

void skeleton_combined_render_model_instance_cook::delete_resource(
	resources::resource_base* resource
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f360]
	DELETE( resource );
}

skeleton_combined_model_instance_cook::skeleton_combined_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_combined_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f1c0]
}

void skeleton_combined_model_instance_cook::translate_query(
	resources::query_result_for_cook&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x650950]
}

void skeleton_combined_model_instance_cook::delete_resource(
	resources::resource_base* resource
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f320]
	DELETE( resource );
}

void skeleton_combined_model_instance_cook::on_resources_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64f590]
}

} // namespace render
} // namespace vostok
