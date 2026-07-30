#include "pch.h"
#include "portal_sector_structure_cook.h"

#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace render {
namespace culling {

portal_sector_structure_cook::portal_sector_structure_cook( ) :
	resources::translate_query_cook(
		resources::portal_sector_structure_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x76aa60]
}

void portal_sector_structure_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76ab00]
}

void portal_sector_structure_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x76aac0]
	portal_sector_structure* structure = static_cast<portal_sector_structure*>( resource );
	DELETE( structure );
}

} // namespace culling
} // namespace render
} // namespace vostok
