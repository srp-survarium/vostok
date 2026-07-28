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
	if ( data.size( ) != 1 )
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

// claude@NOTE: parked at structure level. Target has 5 statements / 2 locals (the
// params only): line 64 (4-byte trivial), line 76 (the whole `new rifle_scope(...)`
// construction reading the two models + 4 config fields), then set_unmanaged_resource
// at line 78 and finish_query at line 79 as SEPARATE statements with NO extra named
// local for the rifle_scope* or the parent. The asm reads the ["data"] node once and
// reuses it, fetches results.get_parent_query() once (held in edi across set+finish),
// and the new pointer flows through eax with no slot. The exact source line-split that
// yields the new (76) separate from the set call (78) while keeping 0 extra locals
// isn't reproduced by the inline form below. NEXT: find the construct (likely a single
// multi-line set_unmanaged_resource( new ... ) whose PDB line table breaks at 76/78)
// that emits the 5-statement shape without a named rifle_scope*/parent local.
void rifle_scope_cook::on_subresources_loaded( resources::queries_result& results, configs::binary_config_ptr const& config )
{
	configs::binary_config_value const& data = config->get_root( )["data"];

	results.get_parent_query( )->set_unmanaged_resource(
		VOSTOK_NEW_IMPL( g_allocator, rifle_scope )(
			static_cast_resource_ptr< render::static_model_ptr >( results[0].get_unmanaged_resource( ) ),
			static_cast_resource_ptr< render::static_model_ptr >( results[1].get_unmanaged_resource( ) ),
			(float)data["change_scope_factor"],
			(bool)data["hide_weapon_on_aim"],
			(float)data["fov_factor"],
			(float)data["near_plane_factor"]
		),
		resources::nocache_memory,
		sizeof( rifle_scope )
	);

	results.get_parent_query( )->finish_query( result_success );
}

} // namespace survarium
