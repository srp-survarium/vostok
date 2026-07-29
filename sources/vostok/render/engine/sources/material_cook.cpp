#include "pch.h"
#include "material_cook.h"
#include "material.h"

namespace vostok {
namespace render {

material_cook::material_cook( ) :
	resources::translate_query_cook(
		resources::material_class,
		reuse_true,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b330]
}

void material_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b510]
}

void material_cook::on_material_config_loaded( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b490]
}

void material_cook::on_material_binary_config_loaded(
	resources::query_result_for_cook*,
	configs::binary_config*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b3e0]
}

void material_cook::delete_resource( resources::resource_base* resource )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76b3a0]
	material* material_resource = static_cast< material* >( resource );
	DELETE( material_resource );
}

} // namespace render
} // namespace vostok
