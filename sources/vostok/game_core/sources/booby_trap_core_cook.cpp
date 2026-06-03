////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_core_cook.h>

namespace survarium {

// STATE[66.69%|DONE]
booby_trap_core_cook::booby_trap_core_cook( ) :
	resources::translate_query_cook( resources::booby_trap_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY[0x761d50]: 1
	// <0x761d85>|0x035|+0x00c:'17'
	// ******
}

// STATE[73.88%|PARTIAL]
void booby_trap_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	configs::binary_config_ptr config;
	if ( !parent.user_data( )->try_get( config ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		parent.finish_query( result_error );
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION );
	pcstr model = (pcstr)config->get_root( )["data"]["model_armed"];

	fixed_string< 260 > aabb_path;
	aabb_path.assignf( "resources/models/%s.model/render/export_properties", model );

	resources::request requests[1] = {
		{ aabb_path.c_str( ), resources::binary_config_class_impl },
	};

	resources::query_resources(
		requests,
		boost::bind( &booby_trap_core_cook::on_subresources_loaded, this, _1, config ),
		g_allocator,
		NULL,
		&parent,
		assert_on_fail_true
	);

	// FUNCTION BODY[0x761ee0]: 26
	// <0x761ef0>|0x010|+0x00b:'22'
	// <0x761efb>|0x01b|+0x01b:'23'
	// <0>
	// <0x761f16>|0x036|+0x012:'25'
	// <0x761f28>|0x048|+0x00c:'26'
	// <0x761f34>|0x054|+0x010:'27'
	// <0>
	// <1>
	// <0x761f44>|0x064|+0x012:'30'
	// <0x761f56>|0x076|+0x033:'31'
	// <0>
	// <1>
	// <2>
	// <0x761f89>|0x0a9|+0x00b:'35'
	// <0x761f94>|0x0b4|+0x01b:'36'
	// <0>
	// <0x761faf>|0x0cf|+0x01a:'38'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x761fc9>|0x0e9|+0x090:'47'
	// ******
}

// STATE[87.99%|DONE]
void booby_trap_core_cook::on_subresources_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	configs::binary_config_ptr aabb = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );

	booby_trap_core* resource = new_derived_resource( );
	resource->load( config->get_root( )["data"] ); // sushi@TODO: Understand;
	resource->load_aabb( aabb->get_root( ) );

	query_for_derived_resources( data.get_parent_query( ), resource, config );


	// FUNCTION BODY[0x761e00]: 12
	// <0x761e0a>|0x00a|+0x00c:'52'
	// <0x761e16>|0x016|+0x00c:'53'
	// <0>
	// <0x761e22>|0x022|+0x022:'55'
	// <0>
	// <1>
	// <0x761e44>|0x044|+0x010:'58'
	// <0x761e54>|0x054|+0x031:'59'
	// <0x761e85>|0x085|+0x016:'60'
	// <0>
	// <1>
	// <0x761e9b>|0x09b|+0x026:'63'
	// ******
}

// STATE[74.00%|DONE]
void booby_trap_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_core* resource )
{
	parent->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( resource ),
		resources::memory_usage_type( resources::nocache_memory, get_derived_resource_size( ) )
	);
	parent->finish_query( result_success );

	// FUNCTION BODY[0x761da0]: 2
	// <0x761da9>|0x009|+0x036:'68'
	// <0x761ddf>|0x03f|+0x00c:'69'
	// ******
}

} // namespace survarium