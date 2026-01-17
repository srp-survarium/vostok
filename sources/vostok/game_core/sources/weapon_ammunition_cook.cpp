////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_ammunition_cook.h>

#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

// STATE[66.69%|PARTIAL]
weapon_ammunition_cook::weapon_ammunition_cook( ) :
	resources::translate_query_cook( resources::weapon_ammunition_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x760585>|0x035|+0x00c:'36'
	// ******
}

// STATE[95.62%|DONE]: LTCG for query_resource.
void weapon_ammunition_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &weapon_ammunition_cook::on_config_ready, this, _1, &parent ),
		g_allocator,
		NULL,
		&parent
	);

	// FUNCTION BODY
	// <0x7606df>|0x00f|+0x00b:'41'
	// <0x7606ea>|0x01a|+0x01d:'42'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x760707>|0x037|+0x122:'51'
	// ******
}

// STATE[31.00%|PARTIAL]
void weapon_ammunition_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// FUNCTION BODY
	// <0x7605a9>|0x009|+0x017:'56'
	// ******
}

// STATE[UNCHECKED]
void weapon_ammunition_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr	config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_ammunition*			wa		= VOSTOK_NEW_IMPL( g_allocator, weapon_ammunition );
	wa->load( config->get_root( )["data"] );

	parent->set_unmanaged_resource( wa, resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_ammunition ) ) );
	parent->finish_query( result_success );

	// FUNCTION BODY
	// <0x7605da>|0x00a|+0x00c:'61'
	// <0x7605e6>|0x016|+0x022:'62'
	// <0x760608>|0x038|+0x051:'63'
	// <0x760659>|0x089|+0x022:'64'
	// <0>
	// <0x76067b>|0x0ab|+0x02d:'66'
	// <0x7606a8>|0x0d8|+0x00c:'67'
	// ******
}

} // namespace survarium
