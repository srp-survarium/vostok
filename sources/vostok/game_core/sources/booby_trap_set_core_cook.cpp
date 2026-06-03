////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_set_core_cook.h>

#include <vostok/game_core/booby_trap_set_core.h>
#include <vostok/game_core/booby_trap_core.h>

namespace survarium {

// STATE[66.69%|DONE]
booby_trap_set_core_cook::booby_trap_set_core_cook( ) :
	resources::translate_query_cook( resources::booby_trap_set_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY[0x762310]: 1
	// <0x762345>|0x035|+0x00c:'18'
	// ******
}

// STATE[90.58%|DONE]
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

	// FUNCTION BODY[0x762750]: 28
	// <0x76275f>|0x00f|+0x00b:'23'
	// <0x76276a>|0x01a|+0x01d:'24'	config_name.assignf( "resources/%s", parent.get_requested_path( ) );
	// <0>
	// <1>
	// <0x762787>|0x037|+0x00c:'27'	if ( parent.user_data( ) )
	// <0>
	// <0x762793>|0x043|+0x01d:'29'		if ( !parent.user_data( )->try_get( cook_data ) )
	// <0>								{
	// <0x7627b0>|0x060|+0x012:'31'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x7627c2>|0x072|+0x00c:'32'			parent.finish_query( result_error );
	// <0x7627ce>|0x07e|+0x005:'33'			return;
	// <0>								}
	// <1>
	// <0x7627d3>|0x083|+0x002:'36'		else
	// <0>								{
	// <1>
	// <0x7627d5>|0x085|+0x007:'39'			cook_data.is_local_player = false;
	// <0x7627dc>|0x08c|+0x007:'40'			cook_data.stack_size = 1;
	// <0>								}
	// <8>
	// <0x7627e3>|0x093|+0x126:'50'		resources::query_resource(
	// ******
}

// STATE[92.27%|DONE]
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

	// FUNCTION BODY[0x7624c0]: 36
	// <0x7624e0>|0x020|+0x00c:'55'
	// <0x7624ec>|0x02c|+0x022:'56'
	// <0x76250e>|0x04e|+0x00c:'57'
	// <0>
	// <0x76251a>|0x05a|+0x03c:'59'
	// <0x762556>|0x096|+0x00c:'60'
	// <0>
	// <0x762562>|0x0a2|+0x010:'62'
	// <0x762572>|0x0b2|+0x00d:'63'	resource->set_amount( cook_data.stack_size );
	// <0x76257f>|0x0bf|+0x00c:'64'	resource->load( data_root )
	// <0>
	// <0x76258b>|0x0cb|+0x008:'66'	if ( cook_data.stack_size == 0 )
	// <0>
	// <0x762593>|0x0d3|+0x015:'68'
	// <0x7625a8>|0x0e8|+0x00d:'69'
	// <0>
	// <1>
	// <0x7625b5>|0x0f5|+0x023:'72'		buffer_vector< resources::request >		requests
	// <0x7625d8>|0x118|+0x023:'73'		buffer_vector< variant< 32 > const* >	user_data
	// <0>
	// <0x7625fb>|0x13b|+0x008:'75'		variant< 32 >	udv;
	// <0x762603>|0x143|+0x00c:'76'		udv.set( config );
	// <0x76260f>|0x14f|+0x027|[1]:'77'	for ( u8 = 0 ; i != cook_data.stack_size ; ++i )
	// <0>								{
	// <0x762636>|0x176|+0x029:'79'
	// <0x76265f>|0x19f|+0x017:'80'
	// <0x762676>|0x1b6|+0x002:'81'		}
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x762678>|0x1b8|+0x0a4:'90'
	// ******
}

// STATE[87.46%|DONE]
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

	// FUNCTION BODY[0x7623c0]: 11
	// <0x7623ca>|0x00a|+0x00c:'98'
	// <0>
	// <0x7623d6>|0x016|+0x023|[1]:'100'
	// <0>
	// <0x7623f9>|0x039|+0x044|[2]:'102'	booby_trap_core_ptr trap = static_cast_resource_ptr< booby_trap_core_ptr >( data[i].get_unmanaged_resource( ) );
	// <0x76243d>|0x07d|+0x00c:'103'		ASSERT( UNKNOWN_EXPRESSION );
	// <0x762449>|0x089|+0x012:'104'		resource->traps( ).push_back( trap );
	// <0x76245b>|0x09b|+0x01e:'105'		trap->set_owner( resource );
	// <0x762479>|0x0b9|+0x00d:'106'	}
	// <0>
	// <0x762486>|0x0c6|+0x02a:'108'
	// ******
}

// STATE[74.00%|DONE]
void booby_trap_set_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_set_core* resource )
{
	parent->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( resource ),
		resources::memory_usage_type( resources::nocache_memory, get_derived_resource_size( ) )
	);
	parent->finish_query( result_success );

	// FUNCTION BODY[0x762360]: 2
	// <0x762369>|0x009|+0x036:'113'
	// <0x76239f>|0x03f|+0x00c:'114'
	// ******
}

} // namespace survarium