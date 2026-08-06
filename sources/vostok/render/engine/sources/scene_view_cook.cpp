#include "pch.h"
#include "scene_view_cook.h"
#include "scene_manager.h"
#include "scene_view.h"

namespace vostok {
namespace render {

scene_view_cook::scene_view_cook( ) :
	super(
		resources::scene_view_class,
		reuse_false,
		use_current_thread_id
	)
{
}

void scene_view_cook::translate_query( resources::query_result_for_cook& parent )
{
	scene_view* created_resource = NEW( scene_view );
	scene_manager::ref( ).add_scene_view( created_resource );

	parent.set_unmanaged_resource( created_resource, resources::nocache_memory, sizeof( scene_view ) );
	parent.finish_query( result_success );
}

void scene_view_cook::delete_resource( resources::resource_base* resource )
{
	scene_view* view = static_cast_checked<scene_view*>( resource );
	scene_manager::ref( ).remove_scene_view( view );

	DELETE( view );
}
} // namespace render
} // namespace vostok
