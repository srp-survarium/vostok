////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_zone_cook.h"
#include "damage_zone.h"
#include <vostok/render/facade/light_props.h>
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/resources_queries_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

damage_zone_cook::damage_zone_cook( game_world& game_world ) :
	resources::translate_query_cook( resources::damage_zone_class, reuse_false, use_current_thread_id ),
	m_game_world( game_world )
{
	resources::register_cook( this );
}

void damage_zone_cook::translate_query( resources::query_result_for_cook& parent )
{
	variant< 32 >* user_data = parent.user_data( );

	configs::binary_config_value cfg_val;
	user_data->try_get( cfg_val );

	vector< resources::request > requests;
	requests.push_back( resources::create_request( "", resources::unknown_data_class ) );

	resources::query_resources(
		&requests.front( ),
		requests.size( ),
		boost::bind( &damage_zone_cook::on_sub_resources_loaded, this, _1, cfg_val ),
		g_allocator,
		NULL,
		&parent
	);
}

// claude@NOTE: The target operations are complete. Remaining flow is resource_ptr
// temporary/inlining codegen, and its load call drops the two unused trailing arguments.
void damage_zone_cook::on_sub_resources_loaded( resources::queries_result& data, configs::binary_config_value const& cfg_val )
{
	damage_zone* zone = VOSTOK_NEW_IMPL( g_allocator, damage_zone )( m_game_world );

	vector< resources::unmanaged_resource_ptr > resources;

	for ( u32 n = data.size( ),
		i = 0; i != n; ++i )
		resources.push_back( data[i].get_unmanaged_resource( ) );

	static u32 start_light_id;
	zone->load( cfg_val, resources, vector< render::light_props >( ), start_light_id );
	start_light_id += 64;

	data.get_parent_query( )->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( zone ),
		resources::memory_usage_type( resources::nocache_memory, sizeof( damage_zone ) )
	);
	data.get_parent_query( )->finish_query( result_success );
}

void damage_zone_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}


} // namespace survarium
