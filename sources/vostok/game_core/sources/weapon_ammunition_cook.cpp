// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_ammunition_cook.h>

#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

weapon_ammunition_cook::weapon_ammunition_cook( ) :
	resources::translate_query_cook( resources::weapon_ammunition_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

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
}

void weapon_ammunition_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

void weapon_ammunition_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	ASSERT( UNKNOWN_EXPRESSION_T( data.size( ) == 1 ) );
	configs::binary_config_ptr	config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_ammunition*			wa		= VOSTOK_NEW_IMPL( g_allocator, weapon_ammunition );
	wa->load( config->get_root( )["data"] );

	parent->set_unmanaged_resource( wa, resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_ammunition ) ) );
	parent->finish_query( result_success );
}

} // namespace survarium
