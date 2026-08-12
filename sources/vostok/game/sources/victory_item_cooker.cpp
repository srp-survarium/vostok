////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "victory_item_cook.h"
#include "victory_item.h"
#include "game_world.h"
#include <vostok/render/facade/model.h>

namespace survarium {

// claude@NOTE: target const-folds `this` for game::register_cooks' function-local
// static cook. The generic construction in anchor_game_victory_item.cpp keeps
// this form generic, but retiring that anchor changes global COMDAT ownership
// and loses exact functions outside this TU. The ctor structure is faithful;
// keep the anchor until the real call graph preserves the complete exact set,
// then remeasure this ctor's constant-folded form. This is an LTCG ownership
// boundary, not a reason to manufacture a second constructor specialization.
victory_item_cook::victory_item_cook( game_world& game_world ) :
	m_game_world( game_world )
{
}

victory_item_core* victory_item_cook::create_resource( )
{
	return VOSTOK_NEW_IMPL( g_allocator, victory_item )( m_game_world );
}

void victory_item_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );

	configs::binary_config_ptr cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );

	victory_item* object_to_cook = static_cast< victory_item* >( create_resource( ) );
	object_to_cook->load( cfg->get_root( ) );

	resources::request requests[1];
	requests[0].id = resources::static_model_instance_class;
	requests[0].path = (pcstr)cfg->get_root( )["model"];

	resources::query_resources(
		requests,
		boost::bind( &victory_item_cook::on_subresources_loaded, this, _1, object_to_cook ),
		g_allocator,
		NULL,
		parent,
		assert_on_fail_true
	);
}

void victory_item_cook::on_subresources_loaded( resources::queries_result& data, victory_item* object_to_cook )
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );

	if ( data.size( ) != 1 )
	{
		LOG_ERROR( "Wrong data in [%s]", data[0].get_requested_path( ) );
		parent->finish_query( resources::query_result_for_user::error_type_cook_failed );
		return;
	}

	object_to_cook->m_model = static_cast_resource_ptr< render::static_model_ptr >( data[0].get_unmanaged_resource( ) );

	parent->set_unmanaged_resource(
		object_to_cook,
		resources::memory_usage_type( resources::nocache_memory, sizeof( victory_item_core ) )
	);

	parent->finish_query( result_success );
}


} // namespace survarium
