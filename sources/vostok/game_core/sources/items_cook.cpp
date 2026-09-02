// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/items_cook.h>

#include <vostok/game_core/medkit.h>
#include <vostok/game_core/oxygen_tank.h>
#include <vostok/game_core/artefact_lifebone_core.h>

namespace survarium {

items_cook::items_cook( ) :
	resources::translate_query_cook( resources::item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

void items_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &items_cook::on_config_ready, this, _1, &parent ),
		g_allocator,
		parent.user_data( ),
		&parent
	);
}

void items_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

void items_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr config		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value current	= config->get_root( )["data"];
	item_types_enum item_type				= (item_types_enum)(u32)current["type"];
	create_item_and_finish_query( item_type, config, parent );
}

void items_cook::create_item_and_finish_query( item_types_enum item_type, configs::binary_config_ptr config, resources::query_result_for_cook* parent )
{
	result_enum result	= result_undefined;

	u16 item_dict_id	= 0;
	if ( parent->user_data( ) )
		parent->user_data( )->try_get<u16>( item_dict_id );

	switch ( item_type )
	{
		case item_type_base_medkit:
		{
			medkit* resource = VOSTOK_NEW_IMPL( g_allocator, medkit );
			resource->load( config->get_root( )["data"] );
			if ( item_dict_id )
				resource->set_dict_id( item_dict_id );

			parent->set_unmanaged_resource( resource, resources::memory_usage_type( resources::nocache_memory, sizeof( medkit ) ) );
			result = result_success;
			break;
		}

		case item_type_oxygen_tank:
		{
			oxygen_tank* resource = VOSTOK_NEW_IMPL( g_allocator, oxygen_tank );
			resource->load( config->get_root( )["data"] );
			parent->set_unmanaged_resource( resource, resources::memory_usage_type( resources::nocache_memory, sizeof( oxygen_tank ) ) );
			result = result_success;
			break;
		}

		case item_type_base_trap:
		{
			ASSERT( UNKNOWN_EXPRESSION );
			result = result_error;
			break;
		}

		case item_type_artefact_lifebone:
		{
			artefact_lifebone_core* resource = VOSTOK_NEW_IMPL( g_allocator, artefact_lifebone_core );
			resource->load_core( config->get_root( )["data"] );
			if ( item_dict_id )
				resource->set_dict_id( item_dict_id );

			parent->set_unmanaged_resource( resource, resources::memory_usage_type( resources::nocache_memory, sizeof( artefact_lifebone_core ) ) );
			result = result_success;
			break;
		}

		// no break and no default clause: the target's switch head keeps the
		// jump-table bounds check (cmp,4; ja end - a NODEFAULT would drop it) and
		// case foo falls out of the switch with no break jmp.
		case item_type_foo:
			result = result_error;
	}

	parent->finish_query( result );
}

} // namespace survarium
