// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/items_dictionary_cook.h>

#include <vostok/game_core/items_dictionary.h>

namespace survarium {

items_dictionary_cook::items_dictionary_cook( ) :
	resources::translate_query_cook( resources::items_dictionary_class, reuse_true, use_any_thread_id )
{
	resources::register_cook( this );
}

void items_dictionary_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource(
		"resources/gameplay/db_static_dictionaries",
		resources::binary_config_class_impl,
		boost::bind( &items_dictionary_cook::on_items_dictionary_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

// claude@NOTE: 0-local target (matches this source). Base differs only in delete_helper arg
// marshalling/order (push edi/pop edi, add esp,4 vs 8) - strip_pointer/delete_helper ceiling on the
// same VOSTOK_DELETE_IMPL source, not steerable.
void items_dictionary_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

void items_dictionary_cook::on_items_dictionary_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const parent	= data.get_parent_query( );
	items_dictionary* cooked_resource				= VOSTOK_NEW_IMPL( g_allocator, items_dictionary );

	cooked_resource->dict_config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const& dict_cfg	= cooked_resource->dict_config->get_root( );

	// claude@NOTE: target calls vectora<request>::vectora(g_allocator) out-of-line; base inlines the
	// ctor. vectora-ctor inline-vs-call ceiling (also strip_pointer on the VOSTOK_NEW_ARRAY_IMPL).
	vectora< resources::request > requests( g_allocator );

	u32 requests_count = dict_cfg["items_dict"].size( );
	u32* item_dict_ids = VOSTOK_NEW_ARRAY_IMPL( g_allocator, u32, requests_count );

	configs::binary_config_value const* items_it	= dict_cfg["items_dict"].begin( );
	configs::binary_config_value const* items_it_e	= dict_cfg["items_dict"].end( );

	for ( u32 i = 0 ; items_it != items_it_e ; ++items_it, ++i )
	{
		pstr item_cfg_path = NULL;
		STR_JOINA( item_cfg_path, "resources/", (*items_it)["cfg_name"] );

		requests.push_back( resources::create_request( item_cfg_path, resources::binary_config_class_impl ) );

		const u32	item_dict_id		= (u32)(*items_it)["dict_id"];
		const u8	item_category_id	= (u8)(*items_it)["item_category"];
		const bool 	is_premium			= (bool)(*items_it)["is_premium"];
		const bool 	is_stack			= (bool)(*items_it)["is_stack"];

		item_dict_ids[i] = item_dict_id;

		dictionary_item item_dict;

		item_dict.item_id		= item_dict_id;
		item_dict.item_category = item_category_id;
		item_dict.item_cfg_name = (*items_it)["cfg_name"];
		item_dict.is_premium	= is_premium;
		item_dict.is_stack		= is_stack;

		cooked_resource->m_items_dict[item_dict.item_id] = item_dict;
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &items_dictionary_cook::on_subresources_loaded, this, _1, cooked_resource, item_dict_ids ),
		g_allocator,
		NULL,
		parent
	);
}

void items_dictionary_cook::on_subresources_loaded(
	resources::queries_result&		data,
	items_dictionary*				cooked_resource,
	u32*							item_dict_ids
)
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );
	ASSERT( UNKNOWN_EXPRESSION_T( parent != NULL ) );

	for ( u32 i = 0 ; i < data.size( ) ; ++i )
	{
		configs::binary_config_ptr item_cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[i].get_unmanaged_resource( ) );
		cooked_resource->m_items_dict[item_dict_ids[i]].item_cfg = item_cfg; // sushi@MATCH: get_dictionary didn't work

		dictionary_item& current_item = cooked_resource->m_items_dict[item_dict_ids[i]];
		if ( current_item.is_ammo( ) )
		{
			u32		clip_size	= (u32)item_cfg->get_root( )["parameters"]["clip_size"];
			float	clip_weight	= (float)item_cfg->get_root( )["parameters"]["clip_weight"];
			current_item.weight = clip_weight / clip_size; // sushi@NOTE: Single bullet weight
		}
		else
			current_item.weight = (float)item_cfg->get_root( )["parameters"]["weight"];

		current_item.combat_log_icon = item_cfg->get_root( )["ui_desc"].value_exists( "combat_log_icon" )
			? (u8)item_cfg->get_root( )["ui_desc"]["combat_log_icon"]
			: 0;
	}
	parent->set_unmanaged_resource(
		cooked_resource,
		resources::memory_usage_type( resources::nocache_memory, sizeof( items_dictionary ) )
	);

	parent->finish_query( result_success, assert_on_fail_true );

	VOSTOK_DELETE_ARRAY_IMPL( g_allocator, item_dict_ids );
}

} // namespace survarium
