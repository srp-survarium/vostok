// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/victory_item_core_cook.h>

#include <vostok/game_core/victory_item_core.h>

namespace survarium {

victory_item_core_cook::victory_item_core_cook( ) :
	resources::translate_query_cook( resources::victory_item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

victory_item_core* victory_item_core_cook::create_resource( )
{
	return VOSTOK_NEW_IMPL( g_allocator, victory_item_core );
}

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

void victory_item_core_cook::delete_resource( resources::resource_base* resource )
{
	// Direct form (no VOSTOK_DELETE_IMPL): the target has NO strip_pointer call -
	// the *g_allocator deref is inline. Residual is the shared delete_helper COMDAT's
	// custom EDI convention vs the target's two-stack-arg cdecl (an argument-passing
	// LTCG boundary).
	memory::delete_helper( *g_allocator, resource );
}

} // namespace survarium
