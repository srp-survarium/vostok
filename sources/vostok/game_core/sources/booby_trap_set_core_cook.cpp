// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/booby_trap_set_core_cook.h>

#include <vostok/game_core/booby_trap_set_core.h>
#include <vostok/game_core/booby_trap_core.h>

namespace survarium {

booby_trap_set_core_cook::booby_trap_set_core_cook( ) :
	resources::translate_query_cook( resources::booby_trap_set_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

void booby_trap_set_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	booby_trap_set_cook_data cook_data;
	if ( parent.user_data( ) )
	{
		if ( !parent.user_data( )->try_get( cook_data ) )
		{
			ASSERT( UNKNOWN_EXPRESSION );
			parent.finish_query( result_error );
			return;
		}
	}
	else
	{
		cook_data.is_local_player = false;
		cook_data.stack_size = 1;
	}

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &booby_trap_set_core_cook::on_config_ready, this, _1, cook_data ),
		g_allocator,
		NULL,
		&parent
	);
}

void booby_trap_set_core_cook::on_config_ready(
	resources::queries_result&	data,
	booby_trap_set_cook_data	cook_data
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	ASSERT( UNKNOWN_EXPRESSION );

	configs::binary_config_value data_root = config->get_root( )["data"];
	ASSERT( UNKNOWN_EXPRESSION );

	booby_trap_set_core* const resource = new_derived_resource( );
	resource->set_amount( cook_data.stack_size );
	resource->load( data_root );

	if ( cook_data.stack_size == 0 )
	{
		finish_query( data.get_parent_query( ), resource );
		return;
	}

	buffer_vector< resources::request >		requests	( ALLOCA( cook_data.stack_size * sizeof( resources::request ) ), cook_data.stack_size );
	buffer_vector< variant< 32 > const* >	user_data	( ALLOCA( cook_data.stack_size * sizeof( variant< 32 > const* ) ), cook_data.stack_size );

	variant< 32 > 							udv;
	udv.set( config );
	for ( u8 i = 0 ; i != cook_data.stack_size ; ++i )
	{
		requests.push_back( resources::create_request( "", resources::booby_trap_class ) );
		user_data.push_back( &udv );
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &booby_trap_set_core_cook::on_subresources_loaded, this, _1, resource, cook_data, config ),
		g_allocator,
		user_data.begin( ),
		data.get_parent_query( )
	);
}

void booby_trap_set_core_cook::on_subresources_loaded(
	resources::queries_result&		data,
	booby_trap_set_core*			resource,
	booby_trap_set_cook_data		cook_data,
	configs::binary_config_ptr		config
)
{

	ASSERT( UNKNOWN_EXPRESSION );

	for ( u8 i = 0 ; i != data.size( ) ; ++i )
	{
		booby_trap_core_ptr trap = static_cast_resource_ptr< booby_trap_core_ptr >( data[i].get_unmanaged_resource( ) );
		ASSERT( UNKNOWN_EXPRESSION );
		resource->traps( ).push_back( trap );
		trap->set_owner( resource );
	}

	query_for_derived_resources( data.get_parent_query( ), resource, cook_data, config );
}

void booby_trap_set_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_set_core* resource )
{
	parent->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( resource ),
		resources::memory_usage_type( resources::nocache_memory, get_derived_resource_size( ) )
	);
	parent->finish_query( result_success );
}

} // namespace survarium
