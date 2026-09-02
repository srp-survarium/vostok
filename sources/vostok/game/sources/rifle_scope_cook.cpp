// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "rifle_scope_cook.h"
#include "rifle_scope.h"

namespace survarium {

rifle_scope_cook::rifle_scope_cook( ) :
	resources::translate_query_cook( resources::rifle_scope_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

void rifle_scope_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &rifle_scope_cook::on_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

void rifle_scope_cook::on_config_loaded( resources::queries_result& data )
{
	if ( !data.is_successful( ) )
	{
		data.get_parent_query( )->finish_query( result_error );
		return;
	}

	configs::binary_config_ptr config			= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const& cfg_data	= config->get_root( )["data"];

	resources::request requests[2] = {
		{ (pcstr)cfg_data["idle_model"],	resources::static_model_instance_class },
		{ (pcstr)cfg_data["aimed_model"],	resources::static_model_instance_class },
	};

	resources::query_resources(
		requests,
		boost::bind( &rifle_scope_cook::on_subresources_loaded, this, _1, config ),
		g_allocator,
		NULL,
		data.get_parent_query( ),
		assert_on_fail_true
	);
}

void rifle_scope_cook::on_subresources_loaded( resources::queries_result& results, configs::binary_config_ptr const& config )
{
	render::static_model_ptr idle_scope = static_cast_resource_ptr< render::static_model_ptr >( results[0].get_unmanaged_resource( ) );
	render::static_model_ptr aimed_scope = static_cast_resource_ptr< render::static_model_ptr >( results[1].get_unmanaged_resource( ) );
	configs::binary_config_value const& data = config->get_root( )["data"];

	rifle_scope* scope = VOSTOK_NEW_IMPL( g_allocator, rifle_scope )(
		idle_scope,
		aimed_scope,
		(float)data["change_scope_factor"],
		(bool)data["hide_weapon_on_aim"],
		(float)data["fov_factor"],
		(float)data["near_plane_factor"]
	);

	resources::query_result_for_cook* parent = results.get_parent_query( );
	parent->set_unmanaged_resource(
		scope,
		resources::memory_usage_type( resources::nocache_memory, sizeof( rifle_scope ) )
	);
	parent->finish_query( result_success );
}

} // namespace survarium
