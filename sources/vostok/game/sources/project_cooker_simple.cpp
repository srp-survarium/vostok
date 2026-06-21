////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "project_cooker_simple.h"
#include "game_project.h"
#include <vostok/game_core/base_project.h>
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/resources_queries_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

 project_cooker_simple::project_cooker_simple( bool editor_present ) :
	translate_query_cook( resources::game_project_simple_class, reuse_true, use_any_thread_id ),
	m_editor_present( editor_present )
{
	resources::register_cook( this );
}

// claude@NOTE: STUB body to provide the vtable symbol (the cook ctor emits the
// vtable, which references delete_resource). The real target inlines the whole
// simple_game_project destructor here: it walks the project's pointer vectors
// (collision+render, ladders, damage-zones, victory-items) and the base_point_stats
// rb-tree, freeing each element + the virtual_path_string vector + the project
// itself via the resource vtable. Match requires the full simple_game_project
// layout/dtor (game_project.h). Parked - buildability stub only.
// STATE[STUB]
void project_cooker_simple::delete_resource( resources::resource_base* resource )
{
	DELETE( resource );
}

void project_cooker_simple::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string project_name( parent.get_requested_path( ) );

	fs_new::virtual_path_string game_proj_path;
	game_proj_path.assignf( "%sprojects/%s/client_project", "resources/", project_name.c_str( ) );

	resources::query_resource(
		game_proj_path.c_str( ),
		resources::binary_config_class,
		boost::bind( &project_cooker_simple::on_game_project_loaded, this, _1, &parent ),
		g_allocator,
		NULL,
		&parent
	);
}

void project_cooker_simple::on_game_project_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	R_ASSERT( data.is_successful() );

	configs::binary_config_ptr game_proj_ptr	= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	create_game_objects				( game_proj_ptr, parent );

	parent->finish_query			( result_success );
}

void project_cooker_simple::on_object_loaded(
	game_object_&							__formal,
	simple_game_project*					project,
	resources::query_result_for_cook*		parent_query
)
{
	++project->m_loaded.loaded_count;

	if ( project->m_loaded.visuals_loaded &&
		 project->m_loaded.collision_loaded &&
		 project->m_loaded.loaded_count == project->m_objects.size( ) &&
		 project->m_loaded.all_queried )
	{
		project->resolve_links( );

		parent_query->set_unmanaged_resource(
			resources::unmanaged_resource_ptr( project ),
			resources::memory_usage_type( resources::nocache_memory, sizeof( simple_game_project ) )
		);
		parent_query->finish_query( result_success );
	}
}

// STATE[STUB]
void project_cooker_simple::create_game_objects(
	configs::binary_config_ptr				project_cfg,
	resources::query_result_for_cook*		parent_query
)
{
	// LOCALS
	// vector< variant< 32 > const* > 	ladders_user_data
	// configs::binary_config_value 	render_visuals
	// boost::function< void( game_object_& ) > callback
	// vector< resources::request > 	damage_zones_requests
	// simple_game_project* 			project
	// vector< resources::creation_request > ext_objects_requests
	// fs_new::virtual_path_string 		project_source_path
	// base_game_scene* 				base_game_scene
	// vector< variant< 32 > const* > 	damage_zones_user_data
	// configs::binary_config_value const* it_e
	// u32 								request_idx
	// vector< resources::request > 	ladders_requests
	// vector< resources::request > 	collision_and_render
	// configs::binary_config_value 	collision_objects
	// configs::binary_config_value const* it
	// collision_geometry* 				geometry
	// victory_items_container* 		container
	// resources::request 				r
	// variant< 32 >* 					ud
	// variant< 32 >* 					ud
	// resources::request 				request
	// ******
}

} // namespace survarium
