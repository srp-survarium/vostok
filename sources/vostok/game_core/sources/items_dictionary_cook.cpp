////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/items_dictionary_cook.h>

#include <vostok/game_core/items_dictionary.h>

namespace survarium {

// STATE[66.69%|PARTIAL]
items_dictionary_cook::items_dictionary_cook( ) :
	resources::translate_query_cook( resources::items_dictionary_class, reuse_true, use_any_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x7645e5>|0x035|+0x00c:'17'
	// ******
}

// STATE[95.53%|DONE]
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

	// FUNCTION BODY[0x764d20]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x764d2c>|0x00c|+0x0d0:'28'
	// <0>
	// ******
}

// STATE[31.00%|PARTIAL] sushi@TODO
void items_dictionary_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// FUNCTION BODY[0x764600]: 1
	// <0x764609>|0x009|+0x017:'34'
	// ******
}

// STATE[65.57%|PARTIAL]
void items_dictionary_cook::on_items_dictionary_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const parent	= data.get_parent_query( );
	items_dictionary* cooked_resource				= VOSTOK_NEW_IMPL( g_allocator, items_dictionary );

	cooked_resource->dict_config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) ); // sushi@MATCH: As always, problem here.
	configs::binary_config_value const& dict_cfg	= cooked_resource->dict_config->get_root( );

	vectora< resources::request > requests( g_allocator ); // sushi@MATCH: In target g_allocator was inlined into constructor, in base constructor was inlined

	u32 requests_count = dict_cfg["items_dict"].size( );
	u32* item_dict_ids = VOSTOK_NEW_ARRAY_IMPL( g_allocator, u32, requests_count );

	configs::binary_config_value const* items_it	= dict_cfg["items_dict"].begin( );
	configs::binary_config_value const* items_it_e	= dict_cfg["items_dict"].end( );

	for ( u32 i = 0 ; items_it != items_it_e ; ++items_it, ++i )
	{
		pstr item_cfg_path = NULL;
		STR_JOINA( item_cfg_path, "resources/", (pcstr)(*items_it)["cfg_name"] );	// sushi@MATCH: This is very different

		requests.push_back( resources::create_request( item_cfg_path, resources::binary_config_class_impl ) );	// sushi@MATCH: In target not inlined

		const u32	item_dict_id		= (u32)(*items_it)["dict_id"];
		const u8	item_category_id	= (u8)(*items_it)["item_category"];
		const bool 	is_premium			= (bool)(*items_it)["is_premium"];
		const bool 	is_stack			= (bool)(*items_it)["is_stack"];

		item_dict_ids[i] = item_dict_id;

		dictionary_item item_dict;

		item_dict.item_id		= item_dict_id;
		item_dict.item_category = item_category_id;
		item_dict.item_cfg_name = (pcstr)(*items_it)["cfg_name"];
		item_dict.is_premium	= is_premium;
		item_dict.is_stack		= is_stack;

		cooked_resource->add_item_desc( item_dict ); // sushi@MATCH: Will it be correct?
		// cooked_resource->m_items_dict[item_dict.item_id] = item_dict;
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &items_dictionary_cook::on_subresources_loaded, this, _1, cooked_resource, item_dict_ids ),
		g_allocator,
		NULL,
		parent
	);

	// FUNCTION BODY[0x7648b0]: 54
	// <0x7648c1>|0x011|+0x00b:'40'
	// <0>
	// <0x7648cc>|0x01c|+0x06f:'42'
	// <0>
	// <0x76493b>|0x08b|+0x045:'44'	cooked_resource->dict_config
	// <0>
	// <0x764980>|0x0d0|+0x015:'46'	configs::binary_config_value const& dict_cfg
	// <0>
	// <0x764995>|0x0e5|+0x008:'48'	vectora< resources::request > requests;
	// <0>
	// <0x76499d>|0x0ed|+0x017:'50'	u32 requests_count
	// <0>
	// <0x7649b4>|0x104|+0x022:'52'	u32* item_dict_ids = VOS
	// <0>
	// <0x7649d6>|0x126|+0x017:'54'	configs::binary_config_value const* it
	// <0x7649ed>|0x13d|+0x017:'55' configs::binary_config_value const* it
	// <0>
	// <0x764a04>|0x154|+0x027|[1]:'57'	for ( u32 i = 0 ; items_it != items_it_e ; ++items_it, ++i )
	// <0>
	// <0x764a2b>|0x17b|+0x007|[2]:'59'		char* item_cfg_path = NULL
	// <0x764a32>|0x182|+0x115:'60'			STR_JOINA( item_cfg_path, "resources/", items_it["cfg_name"] );
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x764b47>|0x297|+0x027:'65'			requests.push_back( resources::create_request( item_cfg_path, resources::binary_config_class_impl ) );
	// <0>
	// <1>
	// <0x764b6e>|0x2be|+0x015:'68'
	// <0x764b83>|0x2d3|+0x015:'69'
	// <0x764b98>|0x2e8|+0x017:'70'
	// <0x764baf>|0x2ff|+0x017:'71'			const bool 	is_stack			= (bool)(*items_it)["is_stack"];
	// <0>
	// <0x764bc6>|0x316|+0x00c:'73'			item_dict_ids[i] = item_dict_id;
	// <0>
	// <0x764bd2>|0x322|+0x016:'75'		dictionary_item item_dict;
	// <0>
	// <0x764be8>|0x338|+0x009:'77'
	// <0x764bf1>|0x341|+0x006:'78'
	// <0x764bf7>|0x347|+0x018:'79'
	// <0x764c0f>|0x35f|+0x006:'80'
	// <0x764c15>|0x365|+0x006:'81'		item_dict.stack			= is_stack;
	// <0>
	// <0x764c1b>|0x36b|+0x023:'83'
	// <0x764c3e>|0x38e|+0x010:'84'	}
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x764c4e>|0x39e|+0x0b4:'93'
	// ******
}

// STATE[80.43%|PARTIAL] Feels correct, I didn't check too deeply though.
void items_dictionary_cook::on_subresources_loaded(
	resources::queries_result&		data,
	items_dictionary*				cooked_resource,
	u32*							item_dict_ids
)
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );
	ASSERT( UNKNOWN_EXPRESSION );

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

	VOSTOK_DELETE_IMPL( g_allocator, item_dict_ids );

	// FUNCTION BODY[0x764630]: 30
	// <0x76464a>|0x01a|+0x00b:'98'
	// <0>
	// <0x764655>|0x025|+0x00c:'100'	ASSERT( UNKNOWN_EXPRESSION )
	// <0>
	// <0x764661>|0x031|+0x023|[1]:'102'for ( u32 i = 0 ; i < data.size( ) ; ++i )
	// <0>
	// <0x764684>|0x054|+0x024:'104'		configs::binary_config_ptr item_cfg = static_cast_resource_ptr< configs::binary_config_ptr >( queries[i].get_unmanaged_resource( ) );
	// <0x7646a8>|0x078|+0x026:'105'		cooked_resource->m_items_dict[item_dict_ids[i]].item_cfg = item_cfg;
	// <0>
	// <0x7646ce>|0x09e|+0x01b:'107'		dictionary_item& current_item = cooked_resource->m_items_dict[item_dict_ids[i]];
	// <0>
	// <0x7646e9>|0x0b9|+0x013:'109'		if ( current_item.is_ammo( ) )
	// <0>
	// <0x7646fc>|0x0cc|+0x02d:'111'
	// <0x764729>|0x0f9|+0x02f:'112'
	// <0x764758>|0x128|+0x025:'113'			current_item.weight = clip_weight / clip_size;
	// <0>
	// <0x76477d>|0x14d|+0x002:'115'		else
	// <0x76477f>|0x14f|+0x035:'116'			current_item.weight = (float)item_cfg->get_root( )["parameters"]["weight"];
	// <0>
	// <0x7647b4>|0x184|+0x07a:'118'
	// <0x76482e>|0x1fe|+0x00d:'119'	}
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x76483b>|0x20b|+0x033:'124'
	// <0x76486e>|0x23e|+0x00c:'125'
	// <0>
	// <0x76487a>|0x24a|+0x02e:'127'
	// ******
}

} // namespace survarium