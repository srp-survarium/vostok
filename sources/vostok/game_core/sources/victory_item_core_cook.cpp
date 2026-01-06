////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core_cook.h>

#include <vostok/game_core/victory_item_core.h>

namespace survarium {

// STATE[66.69%|PARTIAL]: LTCG for translate_query_cook
victory_item_core_cook::victory_item_core_cook( ) :
	resources::translate_query_cook( resources::victory_item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x7620a5>|0x035|+0x00c:'15'
	// ******
}

// STATE[83.57%|DONE]: LTCG for malloc
victory_item_core* victory_item_core_cook::create_resource( )
{
	return VOSTOK_NEW_IMPL( g_allocator, victory_item_core );

	// FUNCTION BODY
	// <0x7620f9>|0x009|+0x04e:'20'
	// ******
}

// STATE[95.53%|DONE]: LTCG for query_resource
void victory_item_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource(
		"resources/gameplay/victory_items/default.lua",
		resources::binary_config_class_impl,
		boost::bind( &victory_item_core_cook::on_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);

	// FUNCTION BODY
	// <0>
	// <7>
	// <0x76222c>|0x00c|+0x0d0:'33'
	// <0>
	// ******
}

// STATE[79.67%|DONE]: LTCG for finish_query
void victory_item_core_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* parent = data.get_parent_query( );

	configs::binary_config_ptr cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) ); // sushi@MATCH: operator[] inlined in target

	victory_item_core* object_to_cook = create_resource( );
	object_to_cook->load( cfg->get_root( ) );

	parent->set_unmanaged_resource(
		object_to_cook,
		resources::memory_usage_type( resources::nocache_memory, sizeof( victory_item_core ) )
	);

	parent->finish_query( result_success );


	// FUNCTION BODY
	// <0x76215a>|0x00a|+0x00b:'39'
	// <0>
	// <0x762165>|0x015|+0x022:'41'
	// <0>
	// <0x762187>|0x037|+0x010:'43'
	// <0>
	// <0x762197>|0x047|+0x01b:'45'
	// <0>
	// <0x7621b2>|0x062|+0x045:'47'
	// <0x7621f7>|0x0a7|+0x00c:'48'
	// ******
}

// STATE[33.38%|PARTIAL]: In base some random call and LTCG for delete_helper
void victory_item_core_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// FUNCTION BODY
	// <0x7620c9>|0x009|+0x013:'53'
	// ******
}

} // namespace survarium
