#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (grass_cook/grass_world_class are new-in-target; every legacy cook is the retired unmanaged_cook shape) - matcher-phase work.
#include "grass_cook.h"
#include "grass_world.h"

namespace vostok {
namespace render {

grass_cook::grass_cook( ) :
	resources::translate_query_cook(
		resources::grass_world_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b640]
}

void grass_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b850]
}

void grass_cook::on_layers_loaded( resources::queries_result&, grass_cook_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b6f0]
}

void grass_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b6b0]
	grass_world* world = static_cast<grass_world*>( resource );
	DELETE( world );
}

} // namespace render
} // namespace vostok
