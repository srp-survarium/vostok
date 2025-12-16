////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/items_dictionary_cook.h>

namespace survarium {

// STATE[STUB]
items_dictionary_cook::items_dictionary_cook( ) : resources::translate_query_cook( resources::items_dictionary_class, reuse_true, use_any_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x7645e5>|0x035|+0x00c:'17'
	// ******
}

// STATE[STUB]
// void survarium::items_dictionary_cook::translate_query(vostok::resources::query_result_for_cook&)
void items_dictionary_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY
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

// STATE[STUB]
// void survarium::items_dictionary_cook::delete_resource(vostok::resources::resource_base*)
void items_dictionary_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY
	// <0x764609>|0x009|+0x017:'34'
	// ******
}

// STATE[STUB]
// void survarium::items_dictionary_cook::on_items_dictionary_config_loaded(vostok::resources::queries_result&)
void items_dictionary_cook::on_items_dictionary_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// u32 							requests_count
	// resources::query_result_for_cook* parent
	// configs::binary_config_value const* items_it_e
	// u32* 						item_dict_ids
	// items_dictionary* 			cooked_resource
	// vectora<resources::request> 	requests
	// configs::binary_config_value const& dict_cfg
	// configs::binary_config_value const* items_it
	// u32 							i<1>
	// dictionary_item 				item_dict<2>
	// bool 						is_stack<2>
	// bool 						is_premium<2>
	// char* 						item_cfg_path<2>
	// u32 							item_dict_id<2>
	// u8 							item_category_id<2>
	// strings::detail::tuples 		STR_JOINA_tuples_unique_identifier<3>
	// ******

	// SKIPPED BLOCKS
	// <0x764a4f><3>
	// ******

	// FUNCTION BODY
	// <0x7648c1>|0x011|+0x00b:'40'
	// <0>
	// <0x7648cc>|0x01c|+0x06f:'42'
	// <0>
	// <0x76493b>|0x08b|+0x045:'44'
	// <0>
	// <0x764980>|0x0d0|+0x015:'46'
	// <0>
	// <0x764995>|0x0e5|+0x008:'48'
	// <0>
	// <0x76499d>|0x0ed|+0x017:'50'
	// <0>
	// <0x7649b4>|0x104|+0x022:'52'
	// <0>
	// <0x7649d6>|0x126|+0x017:'54'
	// <0x7649ed>|0x13d|+0x017:'55'
	// <0>
	// <0x764a04>|0x154|+0x027|[1]:'57'
	// <0>
	// <0x764a2b>|0x17b|+0x007|[2]:'59'
	// <0x764a32>|0x182|+0x115:'60'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x764b47>|0x297|+0x027:'65'
	// <0>
	// <1>
	// <0x764b6e>|0x2be|+0x015:'68'
	// <0x764b83>|0x2d3|+0x015:'69'
	// <0x764b98>|0x2e8|+0x017:'70'
	// <0x764baf>|0x2ff|+0x017:'71'
	// <0>
	// <0x764bc6>|0x316|+0x00c:'73'
	// <0>
	// <0x764bd2>|0x322|+0x016:'75'
	// <0>
	// <0x764be8>|0x338|+0x009:'77'
	// <0x764bf1>|0x341|+0x006:'78'
	// <0x764bf7>|0x347|+0x018:'79'
	// <0x764c0f>|0x35f|+0x006:'80'
	// <0x764c15>|0x365|+0x006:'81'
	// <0>
	// <0x764c1b>|0x36b|+0x023:'83'
	// <0x764c3e>|0x38e|+0x010:'84'
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

// STATE[STUB]
// void survarium::items_dictionary_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::items_dictionary*, unsigned int*)
void items_dictionary_cook::on_subresources_loaded( resources::queries_result& data, items_dictionary* cooked_resource, u32* item_dict_ids )
{
	// LOCALS
	// resources::query_result_for_cook* parent
	// u32 							i<1>
	// configs::binary_config_ptr item_cfg<2>
	// dictionary_item& 			current_item<2>
	// u32 							clip_size<3>
	// float 						clip_weight<3>
	// ******

	// SKIPPED BLOCKS
	// <0x76467e><2>
	// <0x7646f6><3>
	// ******

	// FUNCTION BODY
	// <0x76464a>|0x01a|+0x00b:'98'
	// <0>
	// <0x764655>|0x025|+0x00c:'100'
	// <0>
	// <0x764661>|0x031|+0x023|[1]:'102'
	// <0>
	// <0x764684>|0x054|+0x024:'104'
	// <0x7646a8>|0x078|+0x026:'105'
	// <0>
	// <0x7646ce>|0x09e|+0x01b:'107'
	// <0>
	// <0x7646e9>|0x0b9|+0x013:'109'
	// <0>
	// <0x7646fc>|0x0cc|+0x02d:'111'
	// <0x764729>|0x0f9|+0x02f:'112'
	// <0x764758>|0x128|+0x025:'113'
	// <0>
	// <0x76477d>|0x14d|+0x002:'115'
	// <0x76477f>|0x14f|+0x035:'116'
	// <0>
	// <0x7647b4>|0x184|+0x07a:'118'
	// <0x76482e>|0x1fe|+0x00d:'119'
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

	// TYPEDEFS
	typedef
		vostok::resources::request*
		iterator_type;

	// ******

} // namespace survarium
