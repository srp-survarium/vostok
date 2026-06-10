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

// STATE[93.71%|PARTIAL]: by-value resource_ptr temp materialization via inlined operator[] (LTCG)
// STRUCTURE DIFF[target 0x752150 | base 0x565d20]: target 10 / base 10 stmts
// 0x015 <0x22> | 0x015 <0x1d> | configs::binary_config_ptr cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) ); // sushi@MATCH: operator[] inlined in target   SIZE
// <0>         | --          |    EMPTY only target
// --          | <0>         |    EMPTY only base
// .. same ..
// ; aligned 8, size-diffs 1, quantity-diffs 2
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the by-value resource_ptr temp built through the inlined data[0] operator[]; base spills an extra temp slot (frame 0x34 vs 0x30), non-steerable. trail: on_config_loaded.md
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
}

// STATE[33.38%|PARTIAL]: delete_helper macro lowering - base adds an accessor call + differs in arg passing (LTCG)
// STRUCTURE DIFF[target 0x7520c0 | base 0x565c90]: target 1 / base 1 stmts
// 0x009 <0x13> | 0x00a <0x16> | VOSTOK_DELETE_IMPL( g_allocator, resource );   SIZE
// ; aligned 0, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - target inlines strip_pointer(g_allocator) to mov [g_allocator];push and calls delete_helper( alloc, &resource ); base emits strip_pointer as an out-of-line call (the extra call), whole-program LTCG inline-vs-call, non-steerable. trail: delete_resource.md
void victory_item_core_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

} // namespace survarium
