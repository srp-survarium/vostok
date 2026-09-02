// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "render_output_window_cook.h"
#include "render_output_window.h"
#include "scene_manager.h"
#include <vostok/render/facade/common_types.h>
namespace vostok {
namespace render {

render_output_window_cook::render_output_window_cook( ) :
	super(
		resources::render_output_window_class,
		reuse_false,
		use_current_thread_id
	)
{
}

void render_output_window_cook::translate_query( resources::query_result_for_cook& parent )
{
	output_window_configuration window_configuration;
	parent.user_data( )->try_get( window_configuration );

	render_output_window* created_resource = NEW( render_output_window )( window_configuration );
	scene_manager::ref( ).add_render_output_window( created_resource );

	parent.set_unmanaged_resource( created_resource, resources::nocache_memory, sizeof( render_output_window ) );
	parent.finish_query( result_success );
}

void render_output_window_cook::delete_resource( resources::resource_base* resource )
{
	render_output_window* output_window = static_cast_checked<render_output_window*>( resource );
	scene_manager::ref( ).add_render_output_window( output_window );

	DELETE( output_window );
}

} // namespace render
} // namespace vostok
