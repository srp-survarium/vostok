#include "pch.h"
#include "tracer_model_instance_cook.h"
#include "tracer_model_instance.h"

namespace vostok {
namespace render {

tracer_model_instance_cook::tracer_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::tracer_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x76bfc0]
}

void tracer_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76c1c0]
}

void tracer_model_instance_cook::on_model_ready( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76c070]
}

void tracer_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x76c030]
	tracer_model_instance* instance = static_cast<tracer_model_instance*>( resource );
	DELETE( instance );
}

} // namespace render
} // namespace vostok
