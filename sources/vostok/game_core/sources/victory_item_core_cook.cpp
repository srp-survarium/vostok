////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core_cook.h>

#include <vostok/game_core/victory_item_core.h>

namespace survarium {

// STATE[100%|DONE]
victory_item_core_cook::victory_item_core_cook( ) :
	resources::translate_query_cook( resources::victory_item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[100%|DONE]
victory_item_core* victory_item_core_cook::create_resource( )
{
	return VOSTOK_NEW_IMPL( g_allocator, victory_item_core );
}

// STATE[100%|DONE]
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
}

// STATE[100%|DONE]
void victory_item_core_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* parent = data.get_parent_query( );

	configs::binary_config_ptr cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );

	victory_item_core* object_to_cook = create_resource( );
	object_to_cook->load( cfg->get_root( ) );

	parent->set_unmanaged_resource(
		object_to_cook,
		resources::memory_usage_type( resources::nocache_memory, sizeof( victory_item_core ) )
	);

	parent->finish_query( result_success );
}

// STATE[33.38%|PARTIAL]: delete_helper macro lowering - base adds a strip_pointer call + LTCG-promoted convention
void victory_item_core_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE +0x3 | 70 | VOSTOK_DELETE_IMPL( g_allocator, resource );
	// VERDICT: STRUCTURE MATCH - target calls delete_helper cdecl (alloc + &resource pushed); base adds an out-of-line strip_pointer call and LTCG promotes delete_helper to an edi-arg convention, non-steerable.
}

} // namespace survarium
