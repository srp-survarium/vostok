#include "pch.h"
#include "speedtree_cook.h"
#include "speedtree_instance_impl.h"
#include "speedtree_tree.h"

namespace vostok {
namespace render {

speedtree_cook::speedtree_cook( ) :
	resources::translate_query_cook(
		resources::speedtree_class,
		reuse_true,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x769c90]
}

void speedtree_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76a6e0]
}

void speedtree_cook::on_speedtree_raw_data_loaded( resources::queries_result&, speedtree_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76a340]
}

void speedtree_cook::on_model_materials_loaded( resources::queries_result&, speedtree_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76a0f0]
}

void speedtree_cook::query_materail_effects( speedtree_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x769d60]
}

void speedtree_cook::finish_model_creation( speedtree_data* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x769eb0]
}

void speedtree_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x769d20]
	speedtree_tree* tree = static_cast<speedtree_tree*>( resource );
	DELETE( tree );
}

speedtree_instance_cook::speedtree_instance_cook( ) :
	resources::translate_query_cook(
		resources::speedtree_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x769c50]
}

void speedtree_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76a270]
}

void speedtree_instance_cook::on_speedtree_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x769d70]
}

void speedtree_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x769ce0]
	speedtree_instance_impl* instance = static_cast<speedtree_instance_impl*>( resource );
	DELETE( instance );
}

} // namespace render
} // namespace vostok
