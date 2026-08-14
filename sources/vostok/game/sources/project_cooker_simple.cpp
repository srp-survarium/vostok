////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "project_cooker_simple.h"
#include "artefact_container.h"
#include "base_game_scene.h"
#include "game_project.h"
#include "damage_zone.h"
#include "game_object_.h"
#include "generic_anomaly.h"
#include "object_ambient_volume.h"
#include "object_decal.h"
#include "object_environment.h"
#include "object_environment_probe.h"
#include "object_light.h"
#include "object_lpv_occluder.h"
#include "object_particle_visual.h"
#include "object_sky.h"
#include "object_sky_ambient_occlusion.h"
#include "object_sound.h"
#include "object_vegetation.h"
#include "object_volume_fog.h"
#include "object_volumetric_sound.h"
#include "object_wire.h"
#include "victory_items_container.h"
#include "game_memory.h"
#include "render_visual.h"
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/ladder.h>
#include <vostok/game_core/respawn_point_core.h>
#include <vostok/game_core/static_collision.h>
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

void project_cooker_simple::create_game_objects(
	configs::binary_config_ptr				project_cfg,
	resources::query_result_for_cook*		parent_query
)
{
	typedef vector< variant< 32 > const* > user_data_type;
	typedef vector< resources::request > requests_type;

	base_game_scene* base_game_scene = NULL;
	if ( parent_query->user_data( ) )
		parent_query->user_data( )->try_get( base_game_scene );

	fs_new::virtual_path_string project_source_path;
	project_source_path.assignf( "%sprojects/%s", "resources/", parent_query->get_requested_path( ) );

	simple_game_project* project = NEW( simple_game_project )( *base_game_scene );
	project->m_config = project_cfg;

	configs::binary_config_value collision_objects = ( *project_cfg )["collision_objects"];
	project->m_static_collision_objects_count = collision_objects.size( );
	project->m_static_collision_objects = NEW_ARRAY( static_collision, project->m_static_collision_objects_count );

	configs::binary_config_value render_visuals = ( *project_cfg )["render_visuals"];
	project->m_render_visuals_count = render_visuals.size( );
	project->m_render_visuals = NEW_ARRAY( render_visual, project->m_render_visuals_count );

	requests_type collision_and_render;
	collision_and_render.resize( project->m_static_collision_objects_count + project->m_render_visuals_count );

	u32 request_idx = 0;
	u32 i = 0;
	configs::binary_config_value::const_iterator it = collision_objects.begin( );
	configs::binary_config_value::const_iterator it_e = collision_objects.end( );
	for ( ; it != it_e;
		++it, ++i, ++request_idx )
	{
		collision_and_render[request_idx].set( ( *it )["lib_name"], resources::collision_bt_shape_class_static );
		project->m_static_collision_objects[i].filter_group_ = ( *it )["cgroup"];
		project->m_static_collision_objects[i].filter_mask_ = ( *it )["cmask"];
		read_transform( *it, project->m_static_collision_objects[i].matrix_ );
	}

	i = 0;
	it = render_visuals.begin( );
	it_e = render_visuals.end( );
	for ( ; it != it_e;
		++it, ++i, ++request_idx )
	{
		collision_and_render[request_idx].set( ( *it )["lib_name"], resources::static_model_instance_class );
		read_transform( *it, project->m_render_visuals[i].matrix );
	}

	resources::query_resources(
		collision_and_render.begin( ),
		collision_and_render.size( ),
		boost::bind( &project_cooker_simple::on_collision_and_visuals_loaded, this, _1, project ),
		g_allocator,
		NULL,
		parent_query
	);

	it = ( *project_cfg )["objects"].begin( );
	it_e = ( *project_cfg )["objects"].end( );
	boost::function< void( game_object_& ) > callback =
		boost::bind( &project_cooker_simple::on_object_loaded, this, _1, project, parent_query );

	vector< resources::creation_request > ext_objects_requests;
	requests_type damage_zones_requests;
	user_data_type damage_zones_user_data;
	requests_type ladders_requests;
	user_data_type ladders_user_data;

	for ( ; it != it_e; ++it )
	{
		pcstr type = ( *it )["game_object_type"];
		game_object_* resource = NULL;

		if ( strings::equal( "environment", type ) )
			resource = NEW( object_environment )( *base_game_scene );
		else if ( strings::equal( "sky", type ) )
			resource = NEW( object_sky )( *base_game_scene );
		else if ( strings::equal( "decal", type ) )
			resource = NEW( object_decal )( *base_game_scene );
		else if ( strings::equal( "environment_probe", type ) )
			resource = NEW( object_environment_probe )( *base_game_scene );
		else if ( strings::equal( "sky_ambient_occlusion", type ) )
			resource = NEW( object_sky_ambient_occlusion )( *base_game_scene );
		else if ( strings::equal( "ambient_volume", type ) )
			resource = NEW( object_ambient_volume )( *base_game_scene );
		else if ( strings::equal( "lpv_occluder", type ) )
			resource = NEW( object_lpv_occluder )( *base_game_scene );
		else if ( strings::equal( "volume_fog", type ) )
			resource = NEW( object_volume_fog )( *base_game_scene );
		else if ( strings::equal( "light", type ) )
			resource = NEW( object_light )( *base_game_scene );
		else if ( strings::equal( "particle", type ) )
			resource = NEW( object_particle_visual )( *base_game_scene );
		else if ( strings::equal( "collision_geometry", type ) )
		{
			collision_geometry* geometry = NEW( collision_geometry );
			geometry->load( *it );
			project->register_named_object( ( *it )["full_name"], geometry );
			project->m_collision_geometries.push_back( geometry );
			continue;
		}
		else if ( strings::equal( "volumetric_sound", type ) )
			resource = NEW( object_volumetric_sound )( *base_game_scene );
		else if ( strings::equal( "wire_set", type ) )
			resource = NEW( object_wire )( *base_game_scene );
		else if ( strings::equal( "object_sound", type ) )
			resource = NEW( object_sound )( *base_game_scene );
		else if ( strings::equal( "victory_items_container", type ) )
		{
			victory_items_container* container = NEW( victory_items_container )( *base_game_scene );
			container->load( *it );
			project->register_named_object( ( *it )["full_name"], container );
			project->register_object_to_resolve( container, *it );
			project->m_victory_items_containers.push_back( container );
			continue;
		}
		else if ( strings::equal( "human_npc", type ) )
		{
			configs::binary_config_value* config = NEW( configs::binary_config_value )( *it );
			ext_objects_requests.push_back(
				resources::creation_request(
					( *config )["full_name"],
					const_buffer( static_cast< pcvoid >( config ), sizeof( config ) ),
					resources::human_npc_class
				)
			);
			continue;
		}
		else if ( strings::equal( "damage_zone", type ) )
		{
			resources::request r;
			r.set( ( *it )["full_name"], resources::damage_zone_class );
			damage_zones_requests.push_back( r );

			variant< 32 >* ud = NEW( variant< 32 > );
			ud->set( *it );
			damage_zones_user_data.push_back( ud );
			continue;
		}
		else if ( strings::equal( "vegetation", type ) )
			resource = NEW( object_vegetation )( *base_game_scene );
		else if ( strings::equal( "artefact_container", type ) )
		{
			artefact_container* container = NEW( artefact_container )( *base_game_scene );
			container->load( *it );
			project->register_named_object( ( *it )["full_name"], container );
			project->register_object_to_resolve( container, *it );
			project->m_artefact_containers.push_back( container );
			continue;
		}
		else if ( strings::equal( "ladder", type ) )
		{
			resources::request request;
			request.set( ( *it )["full_name"], resources::ladder_class );
			ladders_requests.push_back( request );

			variant< 32 >* ud = NEW( variant< 32 > );
			ud->set( *it );
			ladders_user_data.push_back( ud );
			continue;
		}
		else if ( strings::equal( "generic_anomaly_core", type ) )
		{
			generic_anomaly* anomaly = NEW( generic_anomaly )( *base_game_scene );
			anomaly->load( *it );
			project->register_object_to_resolve( anomaly, *it );
			project->m_anomalies.push_back( anomaly );
			continue;
		}
		else
		{
			LOG_ERROR( "Object loading not implemented: %s", type );
			continue;
		}

		resource->load( *it, project_source_path.c_str( ), callback );
		project->m_objects.push_back( resource );
	}

	if ( project_cfg->get_root( ).value_exists( "respawn_points" ) )
	{
		it = ( *project_cfg )["respawn_points"].begin( );
		it_e = ( *project_cfg )["respawn_points"].end( );
		for ( ; it != it_e; ++it )
		{
			respawn_point_core* point = NEW( respawn_point_core );
			point->load( *it );
			project->m_respawn_points[point->point_id] = point;
		}
	}

	if ( !ext_objects_requests.empty( ) )
		resources::query_create_resources(
			ext_objects_requests.begin( ),
			ext_objects_requests.size( ),
			boost::bind( &simple_game_project::on_npc_loaded, project, _1 ),
			g_allocator
		);

	project->m_loaded.all_queried = true;
	if ( !damage_zones_requests.empty( ) )
	{
		resources::query_resources(
			damage_zones_requests.begin( ),
			damage_zones_requests.size( ),
			boost::bind( &project_cooker_simple::on_damage_zones_loaded, this, project, _1 ),
			g_allocator,
			damage_zones_user_data.begin( ),
			parent_query
		);
		project->m_loaded.all_queried = false;
	}

	if ( !ladders_requests.empty( ) )
	{
		resources::query_resources(
			ladders_requests.begin( ),
			ladders_requests.size( ),
			boost::bind( &project_cooker_simple::on_ladders_loaded, this, project, _1 ),
			g_allocator,
			ladders_user_data.begin( ),
			parent_query
		);
		project->m_loaded.all_queried = false;
	}

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

void project_cooker_simple::delete_resource( resources::resource_base* resource )
{
	simple_game_project* project = static_cast< simple_game_project* >( resource );

	game_object_** it_e = project->m_objects.end( );
	for ( game_object_** it = project->m_objects.begin( ); it != it_e; ++it )
		DELETE( *it );

	for ( map< u32, respawn_point_core*, std::less< u32 > >::iterator it = project->m_respawn_points.begin( );
		it != project->m_respawn_points.end( );
		++it )
		DELETE( it->second );
	project->m_respawn_points.clear( );

	collision_geometry** geom_end = project->m_collision_geometries.end( );
	for ( collision_geometry** it = project->m_collision_geometries.begin( ); it != geom_end; ++it )
		DELETE( *it );

	victory_items_container** vc_it_e = project->m_victory_items_containers.end( );
	for ( victory_items_container** it = project->m_victory_items_containers.begin( ); it != vc_it_e; ++it )
		DELETE( *it );
	project->m_victory_items_containers.clear( );

	DELETE( project );
}

void project_cooker_simple::on_damage_zones_loaded( simple_game_project* project, resources::queries_result& data )
{
	for ( u32 i = 0; i < data.size( ); ++i )
	{
		damage_zone_ptr zone = static_cast_resource_ptr< damage_zone_ptr >( data[i].get_unmanaged_resource( ) );
		project->m_damage_zones.push_back( zone );

		configs::binary_config_value cfg;
		static_cast< resources::query_result_for_cook& >( data[i] ).user_data( )->try_get( cfg );
		project->register_object_to_resolve( zone.c_ptr( ), cfg );
		project->register_named_object( cfg["full_name"], zone.c_ptr( ) );
	}

	project->m_loaded.all_queried = true;
	if ( project->m_loaded.visuals_loaded &&
		 project->m_loaded.collision_loaded &&
		 project->m_loaded.loaded_count == project->m_objects.size( ) )
	{
		project->resolve_links( );
		data.get_parent_query( )->set_unmanaged_resource(
			resources::unmanaged_resource_ptr( project ),
			resources::memory_usage_type( resources::nocache_memory, sizeof( simple_game_project ) )
		);
		data.get_parent_query( )->finish_query( result_success );
	}
}

void project_cooker_simple::on_ladders_loaded( simple_game_project* project, resources::queries_result& data )
{
	for ( u32 i = 0; i < data.size( ); ++i )
	{
		ladder_ptr new_ladder = static_cast_resource_ptr< ladder_ptr >( data[i].get_unmanaged_resource( ) );
		project->m_ladders.push_back( new_ladder );

		configs::binary_config_value cfg;
		static_cast< resources::query_result_for_cook& >( data[i] ).user_data( )->try_get( cfg );
		project->register_object_to_resolve( new_ladder.c_ptr( ), cfg );
		project->register_named_object( cfg["full_name"], new_ladder.c_ptr( ) );
	}

	project->m_loaded.all_queried = true;
	if ( project->m_loaded.visuals_loaded &&
		 project->m_loaded.collision_loaded &&
		 project->m_loaded.loaded_count == project->m_objects.size( ) )
	{
		project->resolve_links( );
		data.get_parent_query( )->set_unmanaged_resource(
			resources::unmanaged_resource_ptr( project ),
			resources::memory_usage_type( resources::nocache_memory, sizeof( simple_game_project ) )
		);
		data.get_parent_query( )->finish_query( result_success );
	}
}

void project_cooker_simple::on_collision_and_visuals_loaded( resources::queries_result& data, simple_game_project* project )
{
	u32 request_idx = 0;
	for ( u32 i = 0; i < project->m_static_collision_objects_count; ++i, ++request_idx )
		project->m_static_collision_objects[i].shape_ =
			static_cast_resource_ptr< physics::bt_collision_shape_ptr >( data[request_idx].get_unmanaged_resource( ) );

	for ( u32 i = 0; i < project->m_render_visuals_count; ++i, ++request_idx )
		project->m_render_visuals[i].model =
			static_cast_resource_ptr< render::static_model_ptr >( data[request_idx].get_unmanaged_resource( ) );

	project->m_loaded.collision_loaded = true;
	project->m_loaded.visuals_loaded = true;
	if ( project->m_loaded.loaded_count == project->m_objects.size( ) && project->m_loaded.all_queried )
	{
		project->resolve_links( );
		data.get_parent_query( )->set_unmanaged_resource(
			resources::unmanaged_resource_ptr( project ),
			resources::memory_usage_type( resources::nocache_memory, sizeof( simple_game_project ) )
		);
		data.get_parent_query( )->finish_query( result_success );
	}
}

} // namespace survarium
